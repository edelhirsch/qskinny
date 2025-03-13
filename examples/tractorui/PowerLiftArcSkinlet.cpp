/******************************************************************************
 * Copyright (C) The authors
 *           SPDX-License-Identifier: BSD-3-Clause
 *****************************************************************************/

#include "PowerLiftArcSkinlet.h"

#include "ArcShadowNode.h"
#include "PowerLiftArc.h"

#include <QskArcMetrics.h>
#include <QskRgbValue.h>
#include <QskSGNode.h>
#include <QskShadowMetrics.h>

#include <QtMath>

using Q = PowerLiftArc;

PowerLiftArcSkinlet::PowerLiftArcSkinlet( QskSkin* skin )
    : QskSkinlet( skin )
{
    setNodeRoles(
    {
        GrooveShadowRole,
        GrooveRole,
        FillRole,
        HandleRole,
    } );
}

QRectF PowerLiftArcSkinlet::subControlRect( const QskSkinnable* skinnable,
                                       const QRectF& contentsRect, QskAspect::Subcontrol subControl ) const
{
    const auto q = static_cast< const Q* >( skinnable );

    if( subControl == Q::Groove
        || subControl == Q::Fill )
    {
        auto r = contentsRect;

        if( q->type() == Q::Type::Back )
        {
            r.setWidth( r.height() );
        }
        else
        {
            r.setX( r.right() - r.height() );
        }

        return r;
    }

    if( subControl == Q::Handle )
    {
        return {};
    }

    return QskSkinlet::subControlRect( skinnable, contentsRect, subControl );
}

QSizeF PowerLiftArcSkinlet::sizeHint( const QskSkinnable* skinnable, Qt::SizeHint, const QSizeF& size ) const
{
    const auto q = static_cast< const Q* >( skinnable );

    const auto r = size.height() / 2.0;

    const auto am = q->arcMetricsHint( Q::Groove );
    const auto cosStart = qCos( qDegreesToRadians( am.startAngle() ) );
    const auto cosEnd = qCos( qDegreesToRadians( am.endAngle() ) );
    const auto cos = qMax( cosStart, cosEnd );

    const auto w = r - qAbs( cos ) * r + qAbs( cos ) * am.thickness();

    return { w, size.height() };
}

QSGNode* PowerLiftArcSkinlet::updateSubNode( const QskSkinnable* skinnable, quint8 nodeRole, QSGNode* node ) const
{
    const auto q = static_cast< const Q* >( skinnable );

    switch( nodeRole )
    {
        case GrooveShadowRole:
        {
            return updateShadowNode( q, node );
        }
        case GrooveRole:
        {
            return updateArcNode( q, node, Q::Groove );
        }
        case FillRole:
        {
            const auto am = q->arcMetricsHint( Q::Groove );
            const qreal start = am.startAngle();
            const qreal span = q->valueAsRatio( q->value() ) * am.spanAngle();

            return updateArcNode( q, node, start, span, Q::Fill );
        }
        case HandleRole:
        {
            return updateBoxNode( q, node, Q::Handle );
        }
    }

    return QskSkinlet::updateSubNode( skinnable, nodeRole, node );
}

QSGNode* PowerLiftArcSkinlet::updateShadowNode( const PowerLiftArc* arc, QSGNode* node ) const
{
    const auto rect = arc->subControlRect( Q::Groove );
    if ( rect.isEmpty() )
        return nullptr;

    const auto color = arc->shadowColorHint( Q::Groove );
    if ( !QskRgb::isVisible( color ) )
        return nullptr;

    auto metricsArc = arc->arcMetricsHint( Q::Groove );
    metricsArc = metricsArc.toAbsolute( rect.size() );

    auto metrics = arc->shadowMetricsHint( Q::Groove );
    metrics = metrics.toAbsolute( rect.size() );

    const auto shadowRect = metrics.shadowRect( rect );
    const auto spreadRadius = metrics.spreadRadius() + 0.5 * metricsArc.thickness();

    auto shadowNode = QskSGNode::ensureNode< ArcShadowNode >( node );
    shadowNode->setShadowData( shadowRect, spreadRadius, metrics.blurRadius(),
        metricsArc.startAngle(), metricsArc.spanAngle(), color );

    return shadowNode;
}

#include "moc_PowerLiftArcSkinlet.cpp"

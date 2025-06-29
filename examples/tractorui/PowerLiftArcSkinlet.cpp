/******************************************************************************
 * Copyright (C) The authors
 *           SPDX-License-Identifier: BSD-3-Clause
 *****************************************************************************/

#include "PowerLiftArcSkinlet.h"

#include "ArcShadowNode.h"
#include "PowerLiftArc.h"

#include <QskArcHints.h>
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
        BoundariesRole,
        GrooveShadowRole,
        GrooveRole,
        FillRole,
        ProgrammedFillRole,
        MinHandleRole,
        MaxHandleRole,
        ValueHandleRole,
    } );
}

QRectF PowerLiftArcSkinlet::subControlRect( const QskSkinnable* skinnable,
                                       const QRectF& contentsRect, QskAspect::Subcontrol subControl ) const
{
    const auto q = static_cast< const Q* >( skinnable );

    if( subControl == Q::Boundaries )
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
    if( subControl == Q::Groove
        || subControl == Q::Fill )
    {
        const auto amb = q->arcMetricsHint( Q::Boundaries );
        const auto s = q->spacingHint( Q::Boundaries );
        const auto w = amb.thickness() + s;
        auto r = q->subControlRect( Q::Boundaries ).marginsRemoved( { w, w, w, w } );

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
    if( subControl == Q::ProgrammedFill )
    {
        auto r = q->subControlRect( Q::Groove );
        const auto w = q->arcMetricsHint( Q::Groove ).thickness() - q->arcMetricsHint( subControl ).thickness();
        QMarginsF m( w, w, w, w );
        r = r.marginsRemoved( m );
        return r;
    }

    if( subControl == Q::ValueHandle )
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
            auto am = q->arcMetricsHint( Q::Groove );
            const qreal start = am.startAngle();
            const qreal span = q->valueAsRatio( q->value() ) * am.spanAngle();

            am.setStartAngle( start );
            am.setSpanAngle( span );

            auto ah = q->arcHints( Q::Fill );
            ah.metrics = am;

            return updateArcNode( q, node, q->subControlRect( Q::Fill ), ah );
        }
        case ProgrammedFillRole:
        {
            auto am = q->arcMetricsHint( Q::ProgrammedFill );
            const qreal start = am.startAngle();
            const qreal span = q->valueAsRatio( q->programmedValue() ) * am.spanAngle();

            am.setStartAngle( start );
            am.setSpanAngle( span );

            auto ah = q->arcHints( Q::ProgrammedFill );
            ah.metrics = am;

            return updateArcNode( q, node, q->subControlRect( Q::ProgrammedFill ), ah );
        }
        case ValueHandleRole:
        {
            return updateBoxNode( q, node, Q::ValueHandle );
        }
        case MinHandleRole:
        {
            return updateBoxNode( q, node, Q::MinHandle );
        }
        case MaxHandleRole:
        {
            return updateBoxNode( q, node, Q::MaxHandle );
        }
        case BoundariesRole:
        {
            auto am = q->arcMetricsHint( Q::Boundaries );
            const auto b = q->boundaries();
            const qreal start = am.startAngle() + q->valueAsRatio( b.lowerBound() ) * am.spanAngle();
            const qreal span = q->valueAsRatio( b.upperBound() - b.lowerBound() ) * am.spanAngle();

            am.setStartAngle( start );
            am.setSpanAngle( span );

            auto ah = q->arcHints( Q::Boundaries);
            ah.metrics = am;

            return updateArcNode( q, node, q->subControlRect( Q::Boundaries ), ah );
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

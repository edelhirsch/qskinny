/******************************************************************************
 * Copyright (C) The authors
 *           SPDX-License-Identifier: BSD-3-Clause
 *****************************************************************************/

#include "SpeedometerSkinlet.h"

#include "ArcShadowNode.h"
#include "RadialTickmarksNode.h"
#include "Speedometer.h"

#include <QskArcHints.h>
#include <QskArcMetrics.h>
#include <QskArcNode.h>
#include <QskFunctions.h>
#include <QskGraduation.h>
#include <QskGraphic.h>
#include <QskRgbValue.h>
#include <QskSGNode.h>
#include <QskShadowMetrics.h>
#include <QskTickmarks.h>

#include <QFontMetricsF>

using Q = Speedometer;

SpeedometerSkinlet::SpeedometerSkinlet( QskSkin* skin )
    : QskSkinlet( skin )
{
    setNodeRoles(
    {
        OuterPanelRole,
        MiddlePanelRole,
        InnerPanelRole,
        IntensityRole,
        TickmarksRole,
        ValueShadowRole,
        ValueRole,
        ValueTextRole,
        UnitTextRole,
    } );
}

QRectF SpeedometerSkinlet::subControlRect( const QskSkinnable* skinnable,
                                       const QRectF& contentsRect, QskAspect::Subcontrol subControl ) const
{
    const auto q = static_cast< const Q* >( skinnable );

    if( subControl == Q::OuterPanel
        || subControl == Q::MiddlePanel
        || subControl == Q::InnerPanel
        || subControl == Q::Intensity )
    {
        const auto s = q->strutSizeHint( subControl );
        QRectF r( { 0, 0 }, s );
        r.moveCenter( contentsRect.center() );

        return r;
    }

    if( subControl == Q::ValueText
        || subControl == Q::UnitText )
    {
        const auto f1 = q->effectiveFont( Q::ValueText );
        const auto f2 = q->effectiveFont( Q::UnitText );
        const QFontMetricsF fm1( f1 );
        const QFontMetricsF fm2( f2 );

        const auto w = qMax( qskHorizontalAdvance( f1, QString::number( q->value(), 'f', 0 ) ),
            qskHorizontalAdvance( f2, q->unitString() ) );
        const auto h1 = fm1.height();
        const auto h2 = fm2.height();

        QRectF r( 0, 0, w, h1 + h2 );
        r.moveCenter( contentsRect.center() );

        if( subControl == Q::ValueText )
        {
            r.setBottom( r.bottom() - h2 );
        }
        else
        {
            r.setTop( r.top() + h1 );
        }

        return r;
    }

    return QskSkinlet::subControlRect( skinnable, contentsRect, subControl );
}

QSizeF SpeedometerSkinlet::sizeHint( const QskSkinnable* skinnable, Qt::SizeHint, const QSizeF& ) const
{
    const auto q = static_cast< const Q* >( skinnable );

    return q->strutSizeHint( Q::OuterPanel );
}

QSGNode* SpeedometerSkinlet::updateSubNode( const QskSkinnable* skinnable, quint8 nodeRole, QSGNode* node ) const
{
    const auto q = static_cast< const Q* >( skinnable );

    switch( nodeRole )
    {
        case OuterPanelRole:
        {
            return updateBoxNode( q, node, Q::OuterPanel );
        }
        case MiddlePanelRole:
        {
            return updateBoxNode( q, node, Q::MiddlePanel );
        }
        case InnerPanelRole:
        {
            return updateBoxNode( q, node, Q::InnerPanel );
        }
        case IntensityRole:
        {
            return q->showIntensity() ? updateArcNode( q, node, Q::Intensity ) : nullptr;
        }
        case TickmarksRole:
        {
            auto ticksNode = static_cast< RadialTickmarksNode* >( node );
            if ( ticksNode == nullptr )
                ticksNode = new RadialTickmarksNode();

            const auto color = q->color( Q::Tickmarks );
            const auto ticksRect = q->subControlRect( Q::InnerPanel );
            const auto arcMetrics = q->arcMetricsHint( Q::Tickmarks );

            auto tickmarks = QskGraduation::divideInterval( q->minimum(), q->maximum(), 10, 15 );

            const auto tickLineWidth = q->metric( Q::Tickmarks );

            ticksNode->update( color, ticksRect, arcMetrics, tickmarks, q->boundaries(), tickLineWidth );

            return ticksNode;
        }
        case ValueShadowRole:
        {
            return updateShadowNode( q, node );
        }
        case ValueRole:
        {
            auto valueNode = QskSGNode::ensureNode< QskArcNode >( node );

            const auto rect = q->subControlRect( Q::InnerPanel );
            QskArcHints hints = q->arcHints( Q::Value );
            const auto ratio = q->valueAsRatio( q->value() ) * hints.metrics.spanAngle();
            hints.metrics.setSpanAngle( ratio );

            return updateArcNode( q, valueNode, rect, hints );
        }
        case ValueTextRole:
        {
            const auto s = QString::number( q->value(), 'f', 0 );
            return QskSkinlet::updateTextNode( q, node, s, Q::ValueText );
        }
        case UnitTextRole:
        {
            return QskSkinlet::updateTextNode( q, node, q->unitString(), Q::UnitText );
        }
    }

    return QskSkinlet::updateSubNode( skinnable, nodeRole, node );
}

QSGNode* SpeedometerSkinlet::updateShadowNode( const Speedometer* q, QSGNode* node ) const
{
    const auto rect = q->subControlRect( Q::InnerPanel );
    if ( rect.isEmpty() )
        return nullptr;

    const auto color = q->shadowColorHint( Q::Value );
    if ( !QskRgb::isVisible( color ) )
        return nullptr;

    auto metricsArc = q->arcMetricsHint( Q::Value );
    metricsArc = metricsArc.toAbsolute( rect.size() );

    auto metrics = q->shadowMetricsHint( Q::Value );
    metrics = metrics.toAbsolute( rect.size() );

    const auto shadowRect = metrics.shadowRect( rect );
    const auto spreadRadius = metrics.spreadRadius() + 0.5 * metricsArc.thickness();

    QskArcHints hints = q->arcHints( Q::Value );
    const auto spanAngle = q->valueAsRatio( q->value() ) * hints.metrics.spanAngle();

    auto shadowNode = QskSGNode::ensureNode< ArcShadowNode >( node );
    shadowNode->setShadowData( shadowRect, spreadRadius, metrics.blurRadius(),
        metricsArc.startAngle(), spanAngle, color );

    return shadowNode;
}

#include "moc_SpeedometerSkinlet.cpp"

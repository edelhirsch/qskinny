/******************************************************************************
 * QSkinny - Copyright (C) The authors
 *           SPDX-License-Identifier: BSD-3-Clause
 *****************************************************************************/

#include "PowerLiftArc.h"

#include <QskArcMetrics.h>
#include <QskEvent.h>
#include <QskFunctions.h>

#include <QtMath>

QSK_SUBCONTROL( TractorLabel, Panel )
QSK_SUBCONTROL( TractorLabel, Graphic )

TractorLabel::TractorLabel( const QString& url, QQuickItem* parent )
    : QskGraphicLabel( url, parent )
{
    setSubcontrolProxy( QskGraphicLabel::Panel, Panel );
    setSubcontrolProxy( QskGraphicLabel::Graphic, Graphic );

    setPanel( true );
}


QSK_SUBCONTROL( PowerLiftArc, Boundaries )
QSK_SUBCONTROL( PowerLiftArc, Groove )
QSK_SUBCONTROL( PowerLiftArc, Fill )
QSK_SUBCONTROL( PowerLiftArc, ProgrammedFill )
QSK_SUBCONTROL( PowerLiftArc, ValueHandle )

// FirstUserState is taken by QskBoundedInput::ReadOnly, so shift by one
QSK_STATE( PowerLiftArc, Pressed, ( QskAspect::FirstUserState << 1 ) )

namespace
{
    // standard math angle ( 0° at 3 o'clock, CCW positive ) of a point
    // relative to the center of rect, matching QskArcMetrics conventions
    qreal angleAt( const QRectF& rect, const QPointF& point )
    {
        const auto dx = point.x() - rect.center().x();
        const auto dy = rect.center().y() - point.y(); // flip: screen y grows downwards

        return qRadiansToDegrees( std::atan2( dy, dx ) );
    }
}

PowerLiftArc::PowerLiftArc( Type type, QQuickItem* parent )
    : Inherited( parent )
    , m_type( type )
    , m_programmedValue( minimum() )
{
    setSizePolicy( QskSizePolicy::Constrained, QskSizePolicy::Preferred );

    setBoundaries( 0.0, 1.0 );
    setStepSize( 0.05 );
    setPageSteps( 4 );
}

PowerLiftArc::Type PowerLiftArc::type() const
{
    return m_type;
}

bool PowerLiftArc::isPressed() const
{
    return hasSkinState( Pressed );
}

qreal PowerLiftArc::programmedValue() const
{
    return m_programmedValue;
}

void PowerLiftArc::setProgrammedValue( qreal value )
{
    const auto bv = boundedValue( value );

    if( !qskFuzzyCompare( bv, m_programmedValue ) )
    {
        m_programmedValue = bv;
        update();
        Q_EMIT programmedValueChanged( m_programmedValue );
    }
}

QskAspect::Variation PowerLiftArc::effectiveVariation() const
{
    return static_cast< QskAspect::Variation >( m_type );
}

void PowerLiftArc::mousePressEvent( QMouseEvent* event )
{
    if( isReadOnly() )
    {
        Inherited::mousePressEvent( event );
        return;
    }

    const auto pos = qskMousePosition( event );

    // grab when pressing the handle, or jump to the pressed position when
    // pressing somewhere along the arc itself
    if( subControlRect( ValueHandle ).contains( pos ) )
    {
        setSkinStateFlag( Pressed );
        return;
    }

    const auto ratio = valueAtPoint( pos );
    if( ratio >= 0.0 )
    {
        setSkinStateFlag( Pressed );
        setValueAsRatio( ratio );
        return;
    }

    Inherited::mousePressEvent( event );
}

void PowerLiftArc::mouseMoveEvent( QMouseEvent* event )
{
    if( !isPressed() )
        return;

    const auto ratio = valueAtPoint( qskMousePosition( event ) );

    if( ratio < 0.0 )
        return; // cursor wandered off the arc - keep the value, keep grabbing

    setValueAsRatio( ratio );
}

void PowerLiftArc::mouseReleaseEvent( QMouseEvent* )
{
    setSkinStateFlag( Pressed, false );
}

qreal PowerLiftArc::valueAtPoint( const QPointF& point ) const
{
    const auto rect = subControlRect( Groove );
    if( rect.isEmpty() )
        return -1.0;

    const auto metrics = arcMetricsHint( Groove );
    const auto span = metrics.spanAngle();
    if( qFuzzyIsNull( span ) )
        return -1.0;

    // 1. radial distance has to be close to the groove centre line
    const auto dx = point.x() - rect.center().x();
    const auto dy = rect.center().y() - point.y();
    const auto polarRadius = std::hypot( dx, dy );

    const auto centreRadius = ( rect.width() - metrics.thickness() ) / 2.0;
    const auto tolerance = qMax< qreal >( metrics.thickness(), 24.0 );

    if( qAbs( polarRadius - centreRadius ) > tolerance )
        return -1.0;

    // 2. angle has to fall into the groove span ( with a little slack at the
    // ends so the extreme values stay reachable )
    auto ratio = ( angleAt( rect, point ) - metrics.startAngle() ) / span;

    const auto slack = tolerance / qAbs( span );
    if( ratio < -slack || ratio > 1.0 + slack )
        return -1.0;

    return qBound( 0.0, ratio, 1.0 );
}

#include "moc_PowerLiftArc.cpp"

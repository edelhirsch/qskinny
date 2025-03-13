/******************************************************************************
 * QSkinny - Copyright (C) The authors
 *           SPDX-License-Identifier: BSD-3-Clause
 *****************************************************************************/

#include "PowerLiftArc.h"

#include <QskFunctions.h>

QSK_SUBCONTROL( TractorLabel, Panel )
QSK_SUBCONTROL( TractorLabel, Graphic )

TractorLabel::TractorLabel( const QString& url, QQuickItem* parent )
    : QskGraphicLabel( url, parent )
{
    setSubcontrolProxy( QskGraphicLabel::Panel, Panel );
    setSubcontrolProxy( QskGraphicLabel::Graphic, Graphic );

    setPanel( true );
}


QSK_SUBCONTROL( PowerLiftArc, Groove )
QSK_SUBCONTROL( PowerLiftArc, Fill )
QSK_SUBCONTROL( PowerLiftArc, ProgrammedFill )
QSK_SUBCONTROL( PowerLiftArc, Handle )

PowerLiftArc::PowerLiftArc( Type type, QQuickItem* parent )
    : QskBoundedInput( parent )
    , m_type( type )
    , m_value( minimum() )
    , m_programmedValue( minimum() )
{
    setSizePolicy( QskSizePolicy::Constrained, QskSizePolicy::Preferred );
}

PowerLiftArc::Type PowerLiftArc::type() const
{
    return m_type;
}

qreal PowerLiftArc::value() const
{
    return m_value;
}

void PowerLiftArc::setValue( qreal value )
{
    const auto bv = boundedValue( value );

    if( !qskFuzzyCompare( bv, m_value ) )
    {
        m_value = bv;
        Q_EMIT valueChanged( m_value );
    }
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
        Q_EMIT programmedValueChanged( m_programmedValue );
    }
}

void PowerLiftArc::increment( qreal offset )
{
    setValue( value() + offset );
}

QskAspect::Variation PowerLiftArc::effectiveVariation() const
{
    return static_cast< QskAspect::Variation >( m_type );
}

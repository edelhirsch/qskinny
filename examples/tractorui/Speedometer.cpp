/******************************************************************************
 * QSkinny - Copyright (C) The authors
 *           SPDX-License-Identifier: BSD-3-Clause
 *****************************************************************************/

#include "Speedometer.h"

#include <QskFunctions.h>

QSK_SUBCONTROL( Speedometer, OuterPanel )
QSK_SUBCONTROL( Speedometer, MiddlePanel )
QSK_SUBCONTROL( Speedometer, InnerPanel )
QSK_SUBCONTROL( Speedometer, Tickmarks )
QSK_SUBCONTROL( Speedometer, Value )
QSK_SUBCONTROL( Speedometer, Intensity )
QSK_SUBCONTROL( Speedometer, ValueText )
QSK_SUBCONTROL( Speedometer, UnitText )

class Speedometer::PrivateData
{
  public:
    qreal value = 0.0;
    QString unitString;
    bool showIntensity = false;
};

Speedometer::Speedometer( UnitConversion::UnitType unitType, QQuickItem* parent )
    : QskBoundedControl( parent )
    , UnitConversion( unitType )
    , m_data( new PrivateData )
{
    setBoundaries( 0, 120 );
}

Speedometer::~Speedometer()
{
}

qreal Speedometer::value() const
{
    return m_data->value;
}

void Speedometer::setValue( qreal value )
{
    const auto bv = boundedValue( value );

    if( !qskFuzzyCompare( bv, m_data->value ) )
    {
        m_data->value = bv;
        Q_EMIT valueChanged();
    }
}

bool Speedometer::showIntensity() const
{
    return m_data->showIntensity;
}

void Speedometer::setShowIntensity( bool on )
{
    m_data->showIntensity = on;
}

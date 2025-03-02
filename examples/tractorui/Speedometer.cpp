/******************************************************************************
 * QSkinny - Copyright (C) The authors
 *           SPDX-License-Identifier: BSD-3-Clause
 *****************************************************************************/

#include "Speedometer.h"

#include <QskFunctions.h>

QSK_SUBCONTROL( Speedometer, Panel1 )
QSK_SUBCONTROL( Speedometer, Panel2 )
QSK_SUBCONTROL( Speedometer, Panel3 )
QSK_SUBCONTROL( Speedometer, ValueText )
QSK_SUBCONTROL( Speedometer, UnitText )

class Speedometer::PrivateData
{
  public:
    qreal value = 0.0;
    QString unitString;
};

Speedometer::Speedometer( QQuickItem* parent )
    : QskBoundedControl( parent )
    , m_data( new PrivateData )
{
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
    if( !qskFuzzyCompare( value, m_data->value ) )
    {
        m_data->value = value;
        Q_EMIT valueChanged();
    }
}

QString Speedometer::unitString() const
{
    return m_data->unitString;
}

void Speedometer::setUnitString( const QString& unitString )
{
    m_data->unitString = unitString;
}

/******************************************************************************
 * QSkinny - Copyright (C) The authors
 *           SPDX-License-Identifier: BSD-3-Clause
 *****************************************************************************/

#pragma once

#include "UnitConversion.h"

#include <QskBoundedControl.h>

class Speedometer : public QskBoundedControl, public UnitConversion
{
    Q_OBJECT

  public:
    QSK_SUBCONTROLS( OuterPanel, MiddlePanel, InnerPanel, Tickmarks, Value, Intensity, ValueText, UnitText );

    Speedometer( UnitConversion::UnitType unitType, QQuickItem* parent = nullptr );
    ~Speedometer();

    qreal value() const;
    void setValue( qreal value );

    bool showIntensity() const;
    void setShowIntensity( bool on );

  Q_SIGNALS:
    void valueChanged();

  private:
    class PrivateData;
    std::unique_ptr< PrivateData > m_data;
};

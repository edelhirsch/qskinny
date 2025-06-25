/******************************************************************************
 * QSkinny - Copyright (C) The authors
 *           SPDX-License-Identifier: BSD-3-Clause
 *****************************************************************************/

#pragma once

#include "Tile.h"
#include "UnitConversion.h"

#include <QskGridBox.h>

class QskSlider;

class SliderLabel : public QskTextLabel
{
    Q_OBJECT

  public:
    QSK_SUBCONTROLS( Panel, Text )

    SliderLabel( const QString& text, QQuickItem* parent = nullptr );
};

class LabeledSlider : public QskGridBox, public UnitConversion
{
    Q_OBJECT

  public:
    LabeledSlider( const QString& labelText, UnitConversion::UnitType unitType, const QString& explicitUnitString, QQuickItem* parent = nullptr );

    QskSlider* slider();
    QString valueText() const;
    QString label() const;

  private:
    QskSlider* m_slider;
};

class CabinTile : public Tile
{
    Q_OBJECT

  public:
    CabinTile( QQuickItem* parent = nullptr );
};

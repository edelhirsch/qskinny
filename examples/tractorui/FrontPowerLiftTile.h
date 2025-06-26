/******************************************************************************
 * QSkinny - Copyright (C) The authors
 *           SPDX-License-Identifier: BSD-3-Clause
 *****************************************************************************/

#pragma once

#include "Tile.h"

#include <QskGraphicLabel.h>

class QskSlider;

class FrontPowerLiftTile : public Tile
{
    Q_OBJECT

  public:
    FrontPowerLiftTile( QQuickItem* parent = nullptr );
};

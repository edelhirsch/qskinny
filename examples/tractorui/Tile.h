/******************************************************************************
 * Copyright (C) The authors
 *           SPDX-License-Identifier: BSD-3-Clause
 *****************************************************************************/

#pragma once

#include <QskLinearBox.h>
#include <QskTextLabel.h>

class TileLabel : public QskTextLabel
{
    Q_OBJECT

  public:
    QSK_SUBCONTROLS( Panel, Text )

    TileLabel( const QString& text, QQuickItem* parent );
};

class Tile : public QskLinearBox
{
    Q_OBJECT

  public:
    Tile( QskControl* content, const QString& title, QQuickItem* parent = nullptr );

  protected:
    void updateLayout();

  private:
    TileLabel* const m_label;
};

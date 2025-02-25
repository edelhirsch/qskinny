/******************************************************************************
 * Copyright (C) The authors
 *           SPDX-License-Identifier: BSD-3-Clause
 *****************************************************************************/

#pragma once

#include <QskGraphicLabel.h>
#include <QskLinearBox.h>

class SwitchLabel : public QskGraphicLabel
{
    Q_OBJECT

  public:
    QSK_SUBCONTROLS( Panel, Graphic )

    SwitchLabel( const QString& iconSource, QQuickItem* parent = nullptr );
};

class Switch : public QskLinearBox
{
    Q_OBJECT

  public:
    Switch( const QString& iconSource, QQuickItem* parent = nullptr );
    ~Switch();

  private:
    class PrivateData;
    std::unique_ptr< PrivateData > m_data;
};

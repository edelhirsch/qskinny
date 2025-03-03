/******************************************************************************
 * QSkinny - Copyright (C) The authors
 *           SPDX-License-Identifier: BSD-3-Clause
 *****************************************************************************/

#pragma once

#include <QskPushButton.h>

class QuickAccessButton : public QskPushButton
{
    Q_OBJECT

  public:
    QSK_SUBCONTROLS( Panel, Text, Icon )
    QuickAccessButton( const QString& iconSource, const QString& title, QQuickItem* parent = nullptr );
};

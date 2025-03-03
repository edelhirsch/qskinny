/******************************************************************************
 * QSkinny - Copyright (C) The authors
 *           SPDX-License-Identifier: BSD-3-Clause
 *****************************************************************************/

#include "QuickAccessButton.h"

QSK_SUBCONTROL( QuickAccessButton, Panel )
QSK_SUBCONTROL( QuickAccessButton, Text )
QSK_SUBCONTROL( QuickAccessButton, Icon )

QuickAccessButton::QuickAccessButton( const QString& iconSource, const QString& title, QQuickItem* parent )
    : QskPushButton( title, parent )
{
    setIconSource( iconSource );

    setSubcontrolProxy( QskPushButton::Panel, Panel );
    setSubcontrolProxy( QskPushButton::Text, Text );
    setSubcontrolProxy( QskPushButton::Icon, Icon );

    setSizePolicy( Qt::Vertical, QskSizePolicy::Preferred );
}

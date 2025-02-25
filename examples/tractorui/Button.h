/******************************************************************************
 * Copyright (C) The authors
 *           SPDX-License-Identifier: BSD-3-Clause
 *****************************************************************************/

#pragma once

#include <QskBox.h>
#include <QskPushButton.h>
#include <QskTextLabel.h>

class Button : public QskPushButton
{
        Q_OBJECT

    public:
        QSK_SUBCONTROLS( Panel, InnerPanel1, InnerPanel2, InnerPanel3, Icon, Text )

        Button( const QString& iconSource, QQuickItem* parent = nullptr );
};

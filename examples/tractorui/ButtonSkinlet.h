/******************************************************************************
 * Copyright (C) The authors
 *           SPDX-License-Identifier: BSD-3-Clause
 *****************************************************************************/

#pragma once

#include <QskPushButtonSkinlet.h>

class ButtonSkinlet : public QskPushButtonSkinlet
{
        Q_GADGET

    public:
        enum NodeRole
        {
            InnerPanel1Role = QskPushButtonSkinlet::IconRole + 1,
            InnerPanel2Role,
            InnerPanel3Role,
            RoleCount
        };

        Q_INVOKABLE ButtonSkinlet( QskSkin* = nullptr );

        QRectF subControlRect( const QskSkinnable*,
                               const QRectF&, QskAspect::Subcontrol ) const override;

        QSizeF sizeHint( const QskSkinnable*,
                         Qt::SizeHint, const QSizeF& ) const override;

    protected:
        QSGNode* updateSubNode( const QskSkinnable*,
                                quint8 nodeRole, QSGNode* ) const override;
};

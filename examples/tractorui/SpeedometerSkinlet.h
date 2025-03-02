/******************************************************************************
 * Copyright (C) The authors
 *           SPDX-License-Identifier: BSD-3-Clause
 *****************************************************************************/

#pragma once

#include <QskSkinlet.h>

class SpeedometerSkinlet : public QskSkinlet
{
        Q_GADGET

    public:
        enum NodeRole
        {
            Panel1Role,
            Panel2Role,
            Panel3Role,
            ValueTextRole,
            UnitTextRole,
        };

        Q_INVOKABLE SpeedometerSkinlet( QskSkin* = nullptr );

        QRectF subControlRect( const QskSkinnable*,
                               const QRectF&, QskAspect::Subcontrol ) const override;

        QSizeF sizeHint( const QskSkinnable*,
                         Qt::SizeHint, const QSizeF& ) const override;

    protected:
        QSGNode* updateSubNode( const QskSkinnable*,
                                quint8 nodeRole, QSGNode* ) const override;
};

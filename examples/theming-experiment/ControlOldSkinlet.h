/******************************************************************************
 * QSkinny - Copyright (C) 2021 Uwe Rathmann
 * This file may be used under the terms of the QSkinny License, Version 1.0
 *****************************************************************************/

#ifndef CONTROL_OLD_SKINLET_H
#define CONTROL_OLD_SKINLET_H

#include <QskSkinlet.h>

class ControlOld;

class ControlOldSkinlet : public QskSkinlet
{
        Q_GADGET

        using Inherited = QskSkinlet;

    public:
        enum NodeRole
        {
            Subcontrol1Role,
            Subcontrol2Role,
            Subcontrol3Role,
            Subcontrol4Role,
        };

        Q_INVOKABLE ControlOldSkinlet( QskSkin* = nullptr );
        ~ControlOldSkinlet() override;

        QRectF subControlRect( const QskSkinnable*,
                               const QRectF&, QskAspect::Subcontrol ) const override;

    protected:

        QSGNode* updateSubNode( const QskSkinnable*,
                                quint8 nodeRole, QSGNode* ) const override;
};

#endif // CONTROL_OLD_SKINLET_H

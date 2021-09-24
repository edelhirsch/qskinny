/******************************************************************************
 * Copyright (C) 2022 Edelhirsch Software GmbH
 * This file may be used under the terms of the 3-clause BSD License
 *****************************************************************************/

#pragma once

#include <QskSkinlet.h>

class Cube;

class CubeSkinlet : public QskSkinlet
{
    Q_GADGET

    using Inherited = QskSkinlet;

  public:
    enum NodeRole
    {
        FrontRole,
        LeftRole,
        RightRole,

        RoleCount,
    };

    Q_INVOKABLE CubeSkinlet( QskSkin* = nullptr );
    ~CubeSkinlet() override;

    QRectF subControlRect( const QskSkinnable*,
        const QRectF&, QskAspect::Subcontrol ) const override;

  protected:
    QSGNode* updateSubNode( const QskSkinnable*,
        quint8 nodeRole, QSGNode* ) const override;

  private:
    QSGNode* updateFrontNode( const Cube*, QSGNode* ) const;

    const qreal m_scale = 0.9;
};

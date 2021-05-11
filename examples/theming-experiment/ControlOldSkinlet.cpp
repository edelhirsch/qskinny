/******************************************************************************
 * QSkinny - Copyright (C) 2021 Uwe Rathmann
 * This file may be used under the terms of the QSkinny License, Version 1.0
 *****************************************************************************/

#include "ControlOldSkinlet.h"
#include "ControlOld.h"

ControlOldSkinlet::ControlOldSkinlet( QskSkin* skin )
    : QskSkinlet( skin )
{
    setNodeRoles( { Subcontrol1Role, Subcontrol2Role, Subcontrol3Role,
                    Subcontrol4Role } );
}

ControlOldSkinlet::~ControlOldSkinlet()
{
}

QRectF ControlOldSkinlet::subControlRect(
    const QskSkinnable* /*skinnable*/, const QRectF& /*contentsRect*/,
    QskAspect::Subcontrol subControl ) const
{
    qreal x, y;

    if( subControl == ControlOld::Subcontrol1 )
    {
        x = 0;
        y = 0;
    }
    else if( subControl == ControlOld::Subcontrol2 )
    {
        x = 150;
        y = 0;
    }
    else if( subControl == ControlOld::Subcontrol3 )
    {
        x = 0;
        y = 100;
    }
    else
    {
        x = 150;
        y = 100;
    }

    return QRectF( x, y, 150, 100 );
}

QSGNode* ControlOldSkinlet::updateSubNode(
    const QskSkinnable* skinnable, quint8 nodeRole, QSGNode* node ) const
{
    QskAspect::Subcontrol subcontrol;

    switch( nodeRole )
    {
        case Subcontrol1Role:
            subcontrol = ControlOld::Subcontrol1;
            break;

        case Subcontrol2Role:
            subcontrol = ControlOld::Subcontrol2;
            break;

        case Subcontrol3Role:
            subcontrol = ControlOld::Subcontrol3;
            break;

        default:
            subcontrol = ControlOld::Subcontrol4;
    }

    return updateBoxNode( skinnable, node, subcontrol );
}

#include "moc_ControlOldSkinlet.cpp"

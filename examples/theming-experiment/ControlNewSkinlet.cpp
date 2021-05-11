/******************************************************************************
 * QSkinny - Copyright (C) 2021 Uwe Rathmann
 * This file may be used under the terms of the QSkinny License, Version 1.0
 *****************************************************************************/

#include "ControlNewSkinlet.h"
#include "ControlNew.h"

ControlNewSkinlet::ControlNewSkinlet( QskSkin* skin )
    : QskSkinlet( skin )
{
    setNodeRoles( { Subcontrol1Role, Subcontrol2Role, Subcontrol3Role,
                    Subcontrol4Role } );
}

ControlNewSkinlet::~ControlNewSkinlet()
{
}

QRectF ControlNewSkinlet::subControlRect(
    const QskSkinnable* /*skinnable*/, const QRectF& /*contentsRect*/,
    QskAspect::Subcontrol subControl ) const
{
    qreal x, y;

    if( subControl == ControlNew::Subcontrol1 )
    {
        x = 0;
        y = 0;
    }
    else if( subControl == ControlNew::Subcontrol2 )
    {
        x = 150;
        y = 0;
    }
    else if( subControl == ControlNew::Subcontrol3 )
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

QSGNode* ControlNewSkinlet::updateSubNode(
    const QskSkinnable* skinnable, quint8 nodeRole, QSGNode* node ) const
{
    QskAspect::Subcontrol subcontrol;

    switch( nodeRole )
    {
        case Subcontrol1Role:
            subcontrol = ControlNew::Subcontrol1;
            break;

        case Subcontrol2Role:
            subcontrol = ControlNew::Subcontrol2;
            break;

        case Subcontrol3Role:
            subcontrol = ControlNew::Subcontrol3;
            break;

        default:
            subcontrol = ControlNew::Subcontrol4;
    }

    return updateBoxNode( skinnable, node, subcontrol );
}

#include "moc_ControlNewSkinlet.cpp"

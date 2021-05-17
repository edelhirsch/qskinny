/******************************************************************************
 * QSkinny - Copyright (C) 2021 Uwe Rathmann
 * This file may be used under the terms of the QSkinny License, Version 1.0
 *****************************************************************************/

#include "ControlOldSkinlet.h"
#include "ControlOld.h"

#include <QskTextOptions.h>

ControlOldSkinlet::ControlOldSkinlet( QskSkin* skin )
    : QskSkinlet( skin )
{
    setNodeRoles( { Subcontrol1Role, Subcontrol2Role, Subcontrol3Role,
                    Subcontrol4Role, Text1Role, Text2Role, Text3Role,
                    Text4Role } );
}

ControlOldSkinlet::~ControlOldSkinlet()
{
}

QRectF ControlOldSkinlet::subControlRect(
    const QskSkinnable* /*skinnable*/, const QRectF& /*contentsRect*/,
    QskAspect::Subcontrol subControl ) const
{
    qreal x, y;

    if( subControl == ControlOld::Subcontrol1
        || subControl == ControlOld::Text1 )
    {
        x = 0;
        y = 0;
    }
    else if( subControl == ControlOld::Subcontrol2
             || subControl == ControlOld::Text2 )
    {
        x = 150;
        y = 0;
    }
    else if( subControl == ControlOld::Subcontrol3
             || subControl == ControlOld::Text3 )
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
    switch( nodeRole )
    {
        case Subcontrol1Role:
            return updateBoxNode( skinnable, node, ControlOld::Subcontrol1 );

        case Subcontrol2Role:
            return updateBoxNode( skinnable, node, ControlOld::Subcontrol2 );

        case Subcontrol3Role:
            return updateBoxNode( skinnable, node, ControlOld::Subcontrol3 );

        case Subcontrol4Role:
            return updateBoxNode( skinnable, node, ControlOld::Subcontrol4 );

        case Text1Role:
            return updateTextNode( skinnable, node, "text", {},
                                   ControlOld::Text1 );

        case Text2Role:
            return updateTextNode( skinnable, node, "text", {},
                                   ControlOld::Text2 );

        case Text3Role:
            return updateTextNode( skinnable, node, "text", {},
                                   ControlOld::Text3 );

        case Text4Role:
            return updateTextNode( skinnable, node, "text", {},
                                   ControlOld::Text4 );

        default:
            ;
    }

    return nullptr;
}

#include "moc_ControlOldSkinlet.cpp"

/******************************************************************************
 * QSkinny - Copyright (C) 2021 Uwe Rathmann
 * This file may be used under the terms of the QSkinny License, Version 1.0
 *****************************************************************************/

#include "ControlNewSkinlet.h"
#include "ControlNew.h"

#include <QskPalette.h>
#include <QskTextOptions.h>

ControlNewSkinlet::ControlNewSkinlet( QskSkin* skin )
    : QskSkinlet( skin )
{
    setNodeRoles( { Subcontrol1Role, Subcontrol2Role, Subcontrol3Role,
                    Subcontrol4Role, Text1Role, Text2Role, Text3Role,
                    Text4Role } );
}

ControlNewSkinlet::~ControlNewSkinlet()
{
}

QRectF ControlNewSkinlet::subControlRect(
    const QskSkinnable* /*skinnable*/, const QRectF& /*contentsRect*/,
    QskAspect::Subcontrol subControl ) const
{
    qreal x, y;

    if( subControl == ControlNew::Subcontrol1
        || subControl == ControlNew::Text1 )
    {
        x = 0;
        y = 0;
    }
    else if( subControl == ControlNew::Subcontrol2
             || subControl == ControlNew::Text2 )
    {
        x = 150;
        y = 0;
    }
    else if( subControl == ControlNew::Subcontrol3
             || subControl == ControlNew::Text3 )
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
    switch( nodeRole )
    {
        case Subcontrol1Role:
            return updateBoxNode( skinnable, node,
                                  subControlRect( skinnable, {},
                                                  ControlNew::Subcontrol1 ),
                                  skinnable->gradientHint( QskPalette::Primary ),
                                  ControlNew::Subcontrol1 );

        case Subcontrol2Role:
            return updateBoxNode( skinnable, node,
                                  subControlRect( skinnable, {},
                                                  ControlNew::Subcontrol2 ),
                                  skinnable->gradientHint( QskPalette::Secondary ),
                                  ControlNew::Subcontrol2 );


        case Subcontrol3Role:
            return updateBoxNode( skinnable, node,
                                  subControlRect( skinnable, {},
                                                  ControlNew::Subcontrol3 ),
                                  skinnable->gradientHint( QskPalette::Background ),
                                  ControlNew::Subcontrol3 );


        case Subcontrol4Role:
            return updateBoxNode( skinnable, node,
                                  subControlRect( skinnable, {},
                                                  ControlNew::Subcontrol4 ),
                                  skinnable->gradientHint( QskPalette::Surface ),
                                  ControlNew::Subcontrol4 );

        case Text1Role:
            return updateTextNode( skinnable, node,
                                   subControlRect( skinnable, {},
                                                   ControlNew::Text1 ),
                                   Qt::AlignCenter, "text", {},
                                   ControlNew::Text1 );

        case Text2Role:
            return updateTextNode( skinnable, node,
                                   subControlRect( skinnable, {},
                                                   ControlNew::Text2 ),
                                   Qt::AlignCenter, "text", {},
                                   ControlNew::Text2 );

        case Text3Role:
            return updateTextNode( skinnable, node,
                                   subControlRect( skinnable, {},
                                                   ControlNew::Text3 ),
                                   Qt::AlignCenter, "text", {},
                                   ControlNew::Text3 );

        case Text4Role:
            return updateTextNode( skinnable, node,
                                   subControlRect( skinnable, {},
                                                   ControlNew::Text4 ),
                                   Qt::AlignCenter, "text", {},
                                   ControlNew::Text4 );

        default:
            ;
    }

    return nullptr;
}

#include "moc_ControlNewSkinlet.cpp"

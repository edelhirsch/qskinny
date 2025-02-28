/******************************************************************************
 * Copyright (C) The authors
 *           SPDX-License-Identifier: BSD-3-Clause
 *****************************************************************************/

#include "ButtonSkinlet.h"

#include "Button.h"

#include <QskFunctions.h>
#include <QskGraphic.h>

#include <QFontMetricsF>

using Q = Button;

ButtonSkinlet::ButtonSkinlet( QskSkin* skin )
    : QskPushButtonSkinlet( skin )
{
    setNodeRoles(
    {
        PanelRole,
        InnerPanel1Role,
        InnerPanel2Role,
        InnerPanel3Role,
        IconRole,
        TextRole,
    } );
}

QRectF ButtonSkinlet::subControlRect( const QskSkinnable* skinnable,
                                       const QRectF& contentsRect, QskAspect::Subcontrol subControl ) const
{
    const auto q = static_cast< const Q* >( skinnable );

    if( subControl == Q::Panel )
    {
        if( q->type() != Q::Type::Normal )
            return contentsRect;

        auto r = contentsRect;

        const auto tr = q->subControlRect( Q::Text );
        const auto s = q->spacingHint( Q::Panel );

        r.setBottom( tr.top() - s );

        return r;
    }

    if( subControl == Q::InnerPanel1
        || subControl == Q::InnerPanel2
        || subControl == Q::InnerPanel3
        || subControl == QskPushButton::Icon )
    {
        const auto panelRect = q->subControlRect( Q::Panel );
        const auto s = q->strutSizeHint( subControl );

        QRectF r( {0, 0}, s );
        r.moveCenter( panelRect.center() );

        return r;
    }

    if( subControl == Q::Text )
    {
        if( q->type() != Q::Type::Normal )
            return {};

        auto r = contentsRect;
        const auto f = q->effectiveFont( subControl );

        auto w = qskHorizontalAdvance( f, q->text() );
        r.setWidth( w );
        r.setLeft( contentsRect.left() + ( contentsRect.width() - w ) / 2 );

        auto h = QFontMetricsF( f ).height();
        r.setTop( r.bottom() - h );

        return r;
    }

    return QskPushButtonSkinlet::subControlRect( skinnable, contentsRect, subControl );
}

QSizeF ButtonSkinlet::sizeHint( const QskSkinnable* skinnable, Qt::SizeHint, const QSizeF& ) const
{
    const auto q = static_cast< const Q* >( skinnable );

    const auto sh = q->strutSizeHint( Q::Panel );

    const auto f = q->effectiveFont( Q::Text );
    auto textW = qskHorizontalAdvance( f, q->text() );

    const auto w = qMax( sh.width(), textW );
    auto h = sh.height();

    if( q->type() == Q::Type::Normal )
    {
        const QFontMetricsF fm( f );
        h = sh.height() + q->spacingHint( Q::Panel ) + fm.height();
    }

    return { w, h };
}

QSGNode* ButtonSkinlet::updateSubNode( const QskSkinnable* skinnable, quint8 nodeRole, QSGNode* node ) const
{
    const auto q = static_cast< const Q* >( skinnable );

    switch( nodeRole )
    {
        case PanelRole:
        {
            return updateBoxNode( q, node, Q::Panel );
        }
        case InnerPanel1Role:
        {
            return updateBoxNode( q, node, Q::InnerPanel1 );
        }

        case InnerPanel2Role:
        {
            return updateBoxNode( q, node, Q::InnerPanel2 );
        }

        case InnerPanel3Role:
        {
            return updateBoxNode( q, node, Q::InnerPanel3 );
        }

        case TextRole:
        {
            return QskSkinlet::updateTextNode( q, node, q->text(), Q::Text );
        }
    }

    return QskPushButtonSkinlet::updateSubNode( skinnable, nodeRole, node );
}

#include "moc_ButtonSkinlet.cpp"

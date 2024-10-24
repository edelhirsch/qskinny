/******************************************************************************
 * QSkinny - Copyright (C) The authors
 *           SPDX-License-Identifier: BSD-3-Clause
 *****************************************************************************/

#include "QskSwitchButtonSkinlet.h"
#include "QskSwitchButton.h"

using Q = QskSwitchButton;

static inline qreal qskEffectivePosition( const QskSwitchButton* switchButton )
{
    auto pos = switchButton->positionHint( QskSwitchButton::Handle );
    pos = qBound( 0.0, pos, 1.0 );

    if( switchButton->isInverted() )
        pos = 1.0 - pos;

    if ( switchButton->orientation() == Qt::Horizontal )
    {
        if( switchButton->layoutMirroring() )
            pos = 1.0 - pos;
    }

    return pos;
}

QskSwitchButtonSkinlet::QskSwitchButtonSkinlet( QskSkin* skin )
    : Inherited( skin )
{
    setNodeRoles( { GrooveRole, HandleRole, IconRole, HaloRole } );
}

QskSwitchButtonSkinlet::~QskSwitchButtonSkinlet()
{
}

QRectF QskSwitchButtonSkinlet::subControlRect( const QskSkinnable* skinnable,
    const QRectF& contentsRect, QskAspect::Subcontrol subControl ) const
{
    const auto button = static_cast< const Q* >( skinnable );

    if ( subControl == Q::Groove )
    {
        return grooveRect( button, contentsRect );
    }

    if ( subControl == Q::Handle )
    {
        return handleRect( button, contentsRect );
    }

    if ( subControl == Q::Icon )
    {
        return iconRect( button, contentsRect );
    }

    if ( subControl == Q::Halo )
    {
        return haloRect( button, contentsRect );
    }


    return Inherited::subControlRect( skinnable, contentsRect, subControl );
}

QSizeF QskSwitchButtonSkinlet::sizeHint( const QskSkinnable* skinnable,
    Qt::SizeHint which, const QSizeF& ) const
{
    if ( which != Qt::PreferredSize )
        return QSizeF();

    const auto grooveHint = skinnable->strutSizeHint( QskSwitchButton::Groove );
    const auto handleHint = skinnable->strutSizeHint( QskSwitchButton::Handle );
    const auto haloHint = skinnable->strutSizeHint( QskSwitchButton::Halo );

    auto hint = grooveHint;
    hint = hint.expandedTo( haloHint );
    hint = hint.expandedTo( handleHint );

    return hint;
}

QSGNode* QskSwitchButtonSkinlet::updateSubNode( const QskSkinnable* skinnable,
    quint8 nodeRole, QSGNode* node ) const
{
    switch ( nodeRole )
    {
        case HaloRole:
            return updateBoxNode( skinnable, node, Q::Halo );

        case HandleRole:
            return updateBoxNode( skinnable, node, Q::Handle );

        case GrooveRole:
            return updateBoxNode( skinnable, node, Q::Groove );
    }

    return Inherited::updateSubNode( skinnable, nodeRole, node );
}

QRectF QskSwitchButtonSkinlet::grooveRect(
    const QskSwitchButton* button, const QRectF& contentsRect ) const
{
    auto size = button->strutSizeHint( Q::Groove );

    if ( button->orientation() == Qt::Vertical )
    {
        if ( size.height() < 0.0 )
        {
            const auto handleSize = button->strutSizeHint( Q::Handle );
            size.setHeight( 2 * handleSize.height() );
        }
    }
    else
    {
        if ( size.width() < 0.0 )
        {
            const auto handleSize = button->strutSizeHint( Q::Handle );
            size.setWidth( 2 * handleSize.width() );
        }
    }

    size = size.expandedTo( QSize( 0.0, 0.0 ) );

    QRectF r;
    r.setSize( size );
    r.moveCenter( contentsRect.center() );

    return r;
}

QRectF QskSwitchButtonSkinlet::handleRect(
    const QskSwitchButton* button, const QRectF& contentsRect ) const
{
    const auto grooveRect = subControlRect( button, contentsRect, Q::Groove );
    const auto pos = qskEffectivePosition( button );
    const auto size = button->strutSizeHint( Q::Handle );

    qreal cx, cy;

    if( button->orientation() == Qt::Vertical )
    {
        const qreal y0 = grooveRect.y() + 0.5 * size.height();
        const qreal h = grooveRect.height() - size.height();

        cx = grooveRect.x() + 0.5 * grooveRect.width();
        cy = y0 + pos * h;
    }
    else
    {
        const qreal x0 = grooveRect.x() + 0.5 * size.width();
        const qreal w = grooveRect.width() - size.width();

        cx = x0 + pos * w;
        cy = grooveRect.y() + 0.5 * grooveRect.height();
    }

    QRectF r;
    r.setSize( size );
    r.moveCenter( QPointF( cx, cy ) );

    return r;
}

QRectF QskSwitchButtonSkinlet::iconRect(const QskSwitchButton* button, const QRectF& contentsRect ) const
{
    QRectF rect( { 0, 0 }, button->strutSizeHint( Q::Icon ) );
    const auto hr = handleRect( button, contentsRect );
    rect.moveCenter( hr.center() );
    return rect;
}

QRectF QskSwitchButtonSkinlet::haloRect(
    const QskSwitchButton* button, const QRectF& contentsRect ) const
{
    const auto grooveRect = subControlRect( button, contentsRect, Q::Groove );
    const auto pos = qskEffectivePosition( button );
    const auto sizeHandle = button->strutSizeHint( Q::Handle );
    const auto sizeHalo = button->strutSizeHint( Q::Halo );

    qreal cx, cy;

    if( button->orientation() == Qt::Vertical )
    {
        const qreal y0 = grooveRect.y() + 0.5 * sizeHandle.height();
        const qreal h = grooveRect.height() - sizeHandle.height();

        cx = grooveRect.x() + 0.5 * grooveRect.width();
        cy = y0 + pos * h;
    }
    else
    {
        const qreal x0 = grooveRect.x() + 0.5 * sizeHandle.width();
        const qreal w = grooveRect.width() - sizeHandle.width();

        cx = x0 + pos * w;
        cy = grooveRect.y() + 0.5 * grooveRect.height();
    }

    QRectF r;
    r.setSize( sizeHalo );
    r.moveCenter( QPointF( cx, cy ) );

    return r;
}

#include "moc_QskSwitchButtonSkinlet.cpp"

/******************************************************************************
 * Copyright (C) The authors
 *           SPDX-License-Identifier: BSD-3-Clause
 *****************************************************************************/

#include "Button.h"

#include <QskBoxShapeMetrics.h>
#include <QskGraphic.h>
#include <QskGraphicIO.h>
#include <QskLinearBox.h>
#include <QskInternalMacros.h>
#include <QskSegmentedBar.h>

#include <QQuickWindow>

QSK_QT_PRIVATE_BEGIN
#include <private/qquickitem_p.h>
QSK_QT_PRIVATE_END

QSK_SUBCONTROL( Button, Panel )
QSK_SUBCONTROL( Button, InnerPanel1 )
QSK_SUBCONTROL( Button, InnerPanel2 )
QSK_SUBCONTROL( Button, InnerPanel3 )
QSK_SUBCONTROL( Button, Icon )
QSK_SUBCONTROL( Button, Text )

Button::Button( const QString& iconSource, QQuickItem* parent )
    : QskPushButton( parent )
    , m_type( Type::Normal )
{
    if( !iconSource.isEmpty() )
    {
        setIconSource( iconSource );
    }

    setSizePolicy( Qt::Horizontal, QskSizePolicy::Fixed );

    setSubcontrolProxy( QskPushButton::Panel, Panel );
    setSubcontrolProxy( QskPushButton::Icon, Icon );
    setSubcontrolProxy( QskPushButton::Text, Text );
}

Button::Type Button::type() const
{
    return m_type;
}

void Button::setType( Type type )
{
    m_type = type;
}

QskAspect::Variation Button::effectiveVariation() const
{
    return static_cast< QskAspect::Variation >( m_type );
}

QSK_SUBCONTROL( SidebarOverlayButton, Panel )

SidebarOverlayButton::SidebarOverlayButton( QQuickItem* parent )
    : QskPushButton( parent )
{
    setSubcontrolProxy( QskPushButton::Panel, Panel );
}

QSK_SUBCONTROL( SidebarButtonPopup, Panel )
QSK_SUBCONTROL( SidebarButtonPopup, Overlay )

SidebarButtonPopup::SidebarButtonPopup( Button* button, const QVector< QskLabelData >& options, int index )
    : QskPopup( button )
    , m_button( button )
{
    setSubcontrolProxy( QskPopup::Overlay, Overlay );

    setPopupFlag( PopupFlag::CloseOnPressOutside, true );
    setPlacementPolicy( QskPlacementPolicy::Ignore );
    setAutoLayoutChildren( true );

    setParentItem( window()->contentItem() );

    const auto pos = button->mapToScene( button->contentsRect().topLeft() );

    m_linearBox = new QskLinearBox( Qt::Horizontal, this );
    m_linearBox->setSpacing( 0 );

    auto* b = new SidebarOverlayButton( m_linearBox );
    b->setFixedSize( button->sizeHint() );
    connect( b, &QskPushButton::clicked, this, &QskPopup::close );

    m_bar = new QskSegmentedBar( m_linearBox );
    m_bar->setOptions( options );

    if( index >= 0 )
    {
        m_bar->setSelectedIndex( index );
    }

    setGeometry( pos, sizeHint() );

    connect( this, &QskPopup::fadingChanged, this, &QQuickItem::setClip );
    connect( m_bar, &QskSegmentedBar::selectedIndexChanged, this, &SidebarButtonPopup::selectedIndexChanged );
}

QRectF SidebarButtonPopup::clipRect() const
{
    if ( isFading() && parentItem() )
    {
        const auto sh = effectiveSkinHint( fadingAspect() );
        qreal v;

        if( sh.isValid() )
        {
            v = sh.toDouble();
        }
        else
        {
            v = isOpen() ? 1.0 : 0.0;
        }

        const auto w = v * sizeHint().width();
        QRectF r( 0, 0, w, sizeHint().height() );

        return r;
    }
    return QskPopup::clipRect();
}

QskAspect SidebarButtonPopup::fadingAspect() const
{
    return Panel | QskAspect::Position;
}

void SidebarButtonPopup::updateNode( QSGNode* node )
{
    if ( isFading() && clip() )
    {
        if ( auto clipNode = QQuickItemPrivate::get( this )->clipNode() )
        {
            const auto r = clipRect();

            if ( r != clipNode->rect() )
            {
                clipNode->setRect( r );
                clipNode->update();
            }
        }
    }

    QskPopup::updateNode( node );
}

#include "moc_Button.cpp"

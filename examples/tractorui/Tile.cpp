/******************************************************************************
 * Copyright (C) The authors
 *           SPDX-License-Identifier: BSD-3-Clause
 *****************************************************************************/

#include "Tile.h"

#include <QskTextLabel.h>

QSK_SUBCONTROL( TileLabel, Panel )
QSK_SUBCONTROL( TileLabel, Text )

TileLabel::TileLabel( const QString& text, QQuickItem* parent )
    : QskTextLabel( text, parent )
{
    setSubcontrolProxy( QskTextLabel::Panel, Panel );
    setSubcontrolProxy( QskTextLabel::Text, Text );

    setPanel( true );
}

Tile::Tile( const QString &title, QQuickItem* parent )
    : QskLinearBox( Qt::Vertical, parent )
    , m_label( new TileLabel( title, nullptr ) )
{
    setPanel( true );
    setAutoAddChildren( false );
    setPolishOnResize( true );

    m_label->setParent( this );
    m_label->setParentItem( this );
}

void Tile::setContent( QskControl* content )
{
    if( m_content != nullptr )
    {
        removeItem( m_content );
        m_content->deleteLater();
    }

    content->setSizePolicy( QskSizePolicy::Expanding, QskSizePolicy::Expanding );
    m_content = content;
    addItem( content );
}

void Tile::updateLayout()
{
    QskLinearBox::updateLayout();

    const auto g = geometry();
    const auto h = m_label->sizeHint().height();

    m_label->setGeometry( { 0, g.height() - h }, { g.width(), h } );

    setPadding( { 0, 0, 0, h } );
}

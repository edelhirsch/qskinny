/******************************************************************************
 * QSkinny - Copyright (C) The authors
 *           SPDX-License-Identifier: BSD-3-Clause
 *****************************************************************************/

#include "QuickAccessTile.h"

#include "QuickAccessButton.h"

#include <QskGridBox.h>

QuickAccessTile::QuickAccessTile( QQuickItem* parent )
    : Tile( "quick access", parent )
{
    auto* gridBox = new QskGridBox( this );
    gridBox->setMargins( { 5, 5, 5, 10 } );
    gridBox->setSpacing( 10 );

    QVector< QPair< QString, QString >> tiles = {
        { "network-wired", "devices" },
        { "phabricator", "lighting" },
        { "gas-pump", "fuel" },
        { "life-ring", "tires" },
        { "seedling", "fields" },
        { "map-location", "map" },
    };

    for( int i = 0; i < 6; i++ )
    {
        const auto t = tiles.at( i );
        auto* b = new QuickAccessButton( t.first, t.second );
        gridBox->addItem( b, i / 3, i % 3 );
    }

    setContent( gridBox );
}

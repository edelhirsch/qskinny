/******************************************************************************
 * QSkinny - Copyright (C) The authors
 *           SPDX-License-Identifier: BSD-3-Clause
 *****************************************************************************/

#include "ThemeColorsTile.h"

#include "QuickAccessButton.h"
#include "Skin.h"

#include <QskGridBox.h>
#include <QskSkinManager.h>

ThemeColorsTile::ThemeColorsTile( QQuickItem* parent )
    : Tile( "theme colors", parent )
{
    auto* gridBox = new QskGridBox( this );
    gridBox->setMargins( { 5, 5, 5, 10 } );
    gridBox->setSpacing( 10 );

    QVector< QPair< QRgb, QRgb >> themes = {
        { 0xff002b54, 0xff5cc061 }, // (original)
        { 0xff2cde85, 0xff00414a }, // Qt Company
        { 0xffe20074, 0xffffffff }, // T-Mobile
        { 0xff0166b1, 0xff6f6f6f }, // BMW
        { 0xff009999, 0xff3D3D3B }, // Siemens
        { 0xffE6003A, 0xff0066b2 }, // FC Bayern
        { 0xff367C2B, 0xffFFDE00 }, // John Deere
        { 0xff02154e, 0xffd61233 }, // Tommy Hilfiger
        { 0xff0057AD, 0xffFBDA0C }, // Ikea
    };

    for( int i = 0; i < themes.count(); i++ )
    {
        const auto t = themes.at( i );

        auto* b = new QuickAccessButton( "palette-solid", "theme " + QString::number( i + 1 ) );
        b->setSizePolicy( Qt::Vertical, QskSizePolicy::Preferred );
        b->setColor( QuickAccessButton::Text, t.first );

        connect( b, &QuickAccessButton::clicked, this, [t]()
        {
            QskMaterial3Theme::BaseColors colors;
            colors.primary = t.first;
            colors.secondary = t.second;
            auto* s = new Skin( colors );
            qskSkinManager->setSkin( s );
        } );

        gridBox->addItem( b, i / 3, i % 3 );
    }

    setContent( gridBox );
}

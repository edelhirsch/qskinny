/******************************************************************************
 * QSkinny - Copyright (C) The authors
 *           SPDX-License-Identifier: BSD-3-Clause
 *****************************************************************************/

#include "ThemeColorsTile.h"

#include "QuickAccessButton.h"
#include "Skin.h"

#include <QskGridBox.h>
#include <QskSkinManager.h>

namespace
{
    class Button : public QuickAccessButton
    {
      public:
        Button( const QString& title, QQuickItem* parent = nullptr )
            : QuickAccessButton( "palette-solid", title, parent )
        {
            setSizePolicy( Qt::Vertical, QskSizePolicy::Preferred );
        }

        bool revealLogo() const
        {
            return m_revealLogo;
        }

        void setRevealLogo( bool on )
        {
            m_revealLogo = on;
        }

      private:
        bool m_revealLogo = false;
    };
}

ThemeColorsTile::ThemeColorsTile( QQuickItem* parent )
    : Tile( "theme colors", parent )
{
    auto* gridBox = new QskGridBox( this );
    gridBox->setMargins( { 5, 5, 5, 10 } );
    gridBox->setSpacing( 10 );

    QVector< std::tuple< QString, QRgb, QRgb >> themes = {
        { "logo", 0xff002b54, 0xff5cc061 }, // (original)
        { "logo-Qt", 0xff00414a, 0xff2cde85 }, // Qt Company
        { "logo-T-Mobile", 0xffe20074, 0xff888888 }, // T-Mobile
        { "logo-BMW", 0xff0166b1, 0xff6f6f6f }, // BMW
        { "logo-Siemens", 0xff009999, 0xff3D3D3B }, // Siemens
        { "logo-FC-Bayern", 0xffE6003A, 0xff0066b2 }, // FC Bayern
        { "logo-John-Deere", 0xff367C2B, 0xffFFDE00 }, // John Deere
        { "logo-Tommy-Hilfiger", 0xff02154e, 0xffd61233 }, // Tommy Hilfiger
        { "logo-Ikea", 0xff0057AD, 0xffFBDA0C }, // Ikea
    };

    QVector< Button* > buttons;

    for( int i = 0; i < themes.count(); i++ )
    {
        const auto t = themes.at( i );

        const auto customLogo = std::get< 0 >( t );
        const auto primaryColor = std::get< 1 >( t );
        const auto secondaryColor = std::get< 2 >( t );

        auto* b = new Button( "theme " + QString::number( i + 1 ) );
        buttons.append( b );

        connect( b, &Button::clicked, this, [this, b, buttons, primaryColor, secondaryColor, customLogo]()
        {
            if( !b->revealLogo() )
            {
                QskMaterial3Theme::BaseColors colors;
                colors.primary = primaryColor;
                colors.secondary = secondaryColor;

                auto* s = new Skin( colors );
                qskSkinManager->setSkin( s );

                Q_EMIT logoChanged( { "logo" } );

                for( auto* button : buttons )
                {
                    button->setRevealLogo( button == b );
                }
            }
            else
            {
                Q_EMIT logoChanged( customLogo );
                b->setRevealLogo( false );
            }
        } );

        gridBox->addItem( b, i / 3, i % 3 );
    }

    setContent( gridBox );
}

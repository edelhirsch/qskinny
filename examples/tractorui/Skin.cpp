/******************************************************************************
 * Copyright (C) 2021 Edelhirsch Software GmbH
 *           SPDX-License-Identifier: BSD-3-Clause
 *****************************************************************************/

#include "Skin.h"

#include "Button.h"
#include "ButtonSkinlet.h"

#include "MainBox.h"

#include "Switch.h"

#include <SkinnyNamespace.h>

#include <material3/QskMaterial3Skin.h>

#include <QskColorFilter.h>
#include <QskFontRole.h>
#include <QskRgbValue.h>
#include <QskSeparator.h>
#include <QskShadowMetrics.h>
#include <QskSkinHintTableEditor.h>
#include <QskSwitchButton.h>

#include <QFont>
#include <QFontInfo>

namespace
{
    QFont createFont( qreal pixelSize, QFont::Weight weight )
    {
        QFont font( QStringLiteral( "Calistoga" ), -1, weight );
        font.setPixelSize( pixelSize );

        return font;
    }

    QskGradient backgroundGradient( const QskMaterial3Theme& theme )
    {
        QskGradient g( {
            { 0.0, theme.surfaceVariant },
            { 0.5, theme.onPrimary },
            { 1.0, theme.surfaceVariant },
        } );
        g.setLinearDirection( Qt::Horizontal );

        return g;
    }

    QskGradient buttonGradient( QRgb c )
    {
        QColor c1 = QColor( c ).lighter();
        QColor c2 = QColor( c ).darker();

        QskGradient g( c1, c2 );
        g.setLinearDirection( 0, 0, 1, 1 );

        return g;
    }

    QskGradient buttonGradient( QRgb c1, QRgb c2 )
    {
        QskGradient g( c1, c2 );
        g.setLinearDirection( 0, 0, 1, 1 );

        return g;
    }

    QskGradient sliderGradient( QRgb c )
    {
        QColor c1 = QColor( c ).lighter();
        QColor c2 = QColor( c ).darker();

        QskGradient g( c1, c2 );
        g.setLinearDirection( Qt::Vertical );

        return g;
    }

    QskGradient sliderGradient( QRgb c1, QRgb c2 )
    {
        QskGradient g( c1, c2 );
        g.setLinearDirection( Qt::Vertical );

        return g;
    }
}

class Skin::PrivateData
{
  public:
    PrivateData()
    {
        colors.primary = 0xff002b5e;
        colors.secondary = 0xff5CC061;
        colors.tertiary = 0xff98a6ff;

        theme = new QskMaterial3Theme( QskSkin::LightScheme, colors );
    }

    QskMaterial3Theme::BaseColors colors;
    QskMaterial3Theme* theme;
};

Skin::Skin( QObject* parent )
    : QskSkin( parent )
    , m_data( new PrivateData )
{
    declareSkinlet< Button, ButtonSkinlet >();

    setColorScheme( LightScheme );
}

Skin::~Skin()
{
}

void Skin::initHints()
{
    setupFonts();
    setupGraphicFilters( *m_data->theme );

    QskSkinHintTableEditor ed( &hintTable() );

    const auto& t = *m_data->theme;

    using A = QskAspect;

    {
        using Q = Button;

        // normal buttons:

        ed.setStrutSize( Q::Panel, { 75, 75 } );
        ed.setBoxShape( Q::Panel, 100, Qt::RelativeSize );
        ed.setGradient( Q::Panel, buttonGradient( m_data->colors.secondary ) );
        ed.setShadowColor( Q::Panel, QskRgb::toTransparentF( m_data->colors.secondary, 0.3 ) );
        ed.setShadowMetrics( Q::Panel, 0, 4, { 0, 4 } );
        ed.setShadowMetrics( Q::Panel | Q::Hovered, 2, 4, { 0, 4 } );
        ed.setSpacing( Q::Panel, 5 );

        ed.setStrutSize( Q::InnerPanel1, { 72, 72 } );
        ed.setBoxShape( Q::InnerPanel1, 100, Qt::RelativeSize );

        const auto g = buttonGradient( m_data->theme->primaryContainer, m_data->theme->onPrimary );
        ed.setGradient( Q::InnerPanel1, g );
        ed.setGradient( Q::InnerPanel1 | Q::Pressed, g.reversed() );

        ed.setStrutSize( Q::InnerPanel2, { 67, 67 } );
        ed.setBoxShape( Q::InnerPanel2, 100, Qt::RelativeSize );
        ed.setGradient( Q::InnerPanel2, g.reversed() );
        ed.setGradient( Q::InnerPanel2 | Q::Pressed, g );

        ed.setStrutSize( Q::Icon, { 35, 35 } );
        ed.setGraphicRole( Q::Icon, GraphicRolePrimary );
        ed.setGraphicRole( Q::Icon | Q::Pressed, GraphicRoleOnSurfaceVariant );

        ed.setFontRole( Q::Text, { QskFontRole::Body } );
        ed.setColor( Q::Text, t.primary );


        // sidebar buttons:

        const auto s = Q::Sidebar;

        ed.setStrutSize( Q::Panel | s, { 85, 85 } );
        ed.setBoxShape( Q::Panel | s, 100, Qt::RelativeSize );

        QskGradient sg( t.onPrimary, t.surfaceVariant );
        sg.setLinearDirection( 0, 0, 1, 1 );

        ed.setGradient( Q::Panel | s, sg );
        ed.setGradient( Q::Panel | s | Q::Pressed, sg.reversed() );
        ed.setShadowColor( Q::Panel | s, QskRgb::toTransparentF( t.inverseSurface, 0.2 ) );
        ed.setShadowMetrics( Q::Panel | s, 0, 4, { 0, 4 } );
        ed.setShadowMetrics( Q::Panel | s | Q::Hovered, 2, 4, { 0, 4 } );

        ed.setStrutSize( Q::InnerPanel1 | s, { 75, 75 } );
        ed.setGradient( Q::InnerPanel1 | s, sg.reversed() );

        ed.setStrutSize( Q::InnerPanel2 | s, { 0, 0 } );
    }

    {
        using Q = HeaderElementsBox;
        using R = HeaderElementsBackgroundBox;

        const auto boxShape = 20;
        ed.setBoxShape( Q::Panel, boxShape );
        ed.setBoxShape( R::Panel, boxShape );

        const auto g2 = QskRgb::interpolated( t.surfaceVariant, t.onSecondary, 0.5 );
        QskGradient g( t.surfaceVariant, g2 );
        g.setLinearDirection( Qt::Horizontal );

        ed.setGradient( Q::Panel | A::Left, g );
        ed.setGradient( Q::Panel | A::Right, g.reversed() );

        auto s1 = QskRgb::toTransparentF( m_data->theme->inverseSurface, 0.1 ); // ### own function
        ed.setShadowColor( Q::Panel, s1 );
        ed.setShadowMetrics( Q::Panel, 1, 3, { 1, 1 } );

        auto s2 = QskRgb::toTransparentF( m_data->theme->surface, 0.6 );
        ed.setShadowColor( R::Panel, s2 );
        ed.setShadowMetrics( R::Panel, 1, 3, { -1, -1 } );
    }

    {
        using Q = MainBox;

        ed.setGradient( Q::Panel, backgroundGradient( t ) );
    }

    {
        using Q = SwitchLabel;

        ed.setStrutSize( Q::Graphic, { -1, 46 } );
        ed.setGraphicRole( Q::Graphic, GraphicRolePrimary );
    }

    {
        using Q = QskGraphicLabel;

        ed.setGraphicRole( Q::Graphic, GraphicRolePrimary );
    }

    {
        using Q = QskSeparator;

        ed.setMetric( Q::Panel | A::Size, 1 );
        ed.setGradient( Q::Panel, t.surface );
    }

    {
        using Q = QskSwitchButton;

        ed.setStrutSize( Q::Groove, { 50, 18 } );
        ed.setBoxShape( Q::Groove, 100, Qt::RelativeSize );
        ed.setGradient( Q::Groove, sliderGradient( m_data->theme->primaryContainer, m_data->theme->onPrimary ) );
        ed.setGradient( Q::Groove | Q::Checked, sliderGradient( m_data->theme->secondary ) );

        ed.setStrutSize( Q::Handle, { 30, 30 } );
        ed.setBoxShape( Q::Handle, 100, Qt::RelativeSize );
        ed.setPosition( Q::Handle, 0.0 );
        ed.setPosition( Q::Handle | Q::Checked, 1.0 );
        const auto g = buttonGradient( m_data->theme->primaryContainer, m_data->theme->onPrimary );
        ed.setGradient( Q::Handle, g.reversed() );
        ed.setGradient( Q::Handle | Q::Pressed, g );

        auto c = QskRgb::toTransparentF( m_data->theme->inverseSurface, 0.25 );
        ed.setShadowColor( Q::Handle, c );
        ed.setShadowMetrics( Q::Handle, 2, 4, { 0, 0 } );
    }

    {
        using Q = QskTextLabel;

        ed.setFontRole( Q::Text, { QskFontRole::Body } );
        ed.setColor( Q::Text, t.primary );
    }
}

void Skin::setupFonts()
{
    using F = QskFontRole;

    setFont( { F::Body, F::Normal }, createFont( 16, QFont::Medium ) );

    QskSkin::completeFontTable();
}

void Skin::setGraphicColor( GraphicRole role, QRgb rgb )
{
    QskColorFilter colorFilter;
    colorFilter.setMask( QskRgb::RGBAMask );
    colorFilter.addColorSubstitution( QskRgb::Black, rgb );

    setGraphicFilter( role, colorFilter );
}

void Skin::setupGraphicFilters( const QskMaterial3Theme& theme )
{
    setGraphicColor( GraphicRoleOnSurfaceVariant, theme.onSurfaceVariant );
    setGraphicColor( GraphicRolePrimary, theme.primary );
}

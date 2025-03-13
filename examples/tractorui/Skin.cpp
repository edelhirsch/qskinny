/******************************************************************************
 * Copyright (C) 2021 Edelhirsch Software GmbH
 *           SPDX-License-Identifier: BSD-3-Clause
 *****************************************************************************/

#include "Skin.h"

#include "Button.h"
#include "ButtonSkinlet.h"

#include "CabinTile.h"

#include "MainBox.h"

#include "PowerLiftArc.h"
#include "PowerLiftArcSkinlet.h"

#include "QuickAccessButton.h"

#include "Speedometer.h"
#include "SpeedometerSkinlet.h"

#include "Switch.h"
#include "Tile.h"

#include <SkinnyNamespace.h>

#include <material3/QskMaterial3Skin.h>

#include <QskArcMetrics.h>
#include <QskBoxBorderColors.h>
#include <QskColorFilter.h>
#include <QskFontRole.h>
#include <QskMargins.h>
#include <QskRgbValue.h>
#include <QskSeparator.h>
#include <QskShadowMetrics.h>
#include <QskSlider.h>
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
    declareSkinlet< PowerLiftArc, PowerLiftArcSkinlet >();
    declareSkinlet< Speedometer, SpeedometerSkinlet >();

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

        const auto g = buttonGradient( t.primaryContainer, t.onPrimary );
        ed.setGradient( Q::InnerPanel1, g );
        ed.setGradient( Q::InnerPanel1 | Q::Pressed, g.reversed() );

        ed.setStrutSize( Q::InnerPanel2, { 67, 67 } );
        ed.setBoxShape( Q::InnerPanel2, 100, Qt::RelativeSize );
        ed.setGradient( Q::InnerPanel2, g.reversed() );
        ed.setGradient( Q::InnerPanel2 | Q::Pressed, g );

        ed.setStrutSize( Q::Icon, { 35, 35 } );
        ed.setGraphicRole( Q::Icon, GraphicRolePrimary );
        ed.setGraphicRole( Q::Icon | Q::Pressed, GraphicRoleOnPrimaryContainer );

        ed.setFontRole( Q::Text, { QskFontRole::Body } );
        ed.setColor( Q::Text, t.onPrimaryContainer );


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

        auto s1 = QskRgb::toTransparentF( t.inverseSurface, 0.1 ); // ### own function
        ed.setShadowColor( Q::Panel, s1 );
        ed.setShadowMetrics( Q::Panel, 1, 3, { 1, 1 } );

        auto s2 = QskRgb::toTransparentF( t.surface, 0.6 );
        ed.setShadowColor( R::Panel, s2 );
        ed.setShadowMetrics( R::Panel, 1, 3, { -1, -1 } );
    }

    {
        using Q = MainBox;

        ed.setGradient( Q::Panel, backgroundGradient( t ) );
    }

    {
        using Q = PowerLiftArc;

        ed.setArcMetrics( Q::Groove | Q::Back, { -120, -120, 15 } );
        ed.setArcMetrics( Q::Groove, { -60, 120, 15 } );
        QskGradient groove( { { 0.0, t.primaryContainer }, { 0.4, t.primaryContainer },
            { 0.5, t.onPrimary }, { 1.0, t.onPrimary } } );
        groove.setRadialDirection( 0.5, 0.5, 1 );
        ed.setGradient( Q::Groove, groove );

        auto s1 = QskRgb::toTransparentF( t.inverseSurface, 0.1 ); // ### own function
        ed.setShadowColor( Q::Groove, s1 );
        ed.setShadowMetrics( Q::Groove, 0, 5, { 0, 0 } );

        ed.setArcMetrics( Q::Fill, ed.arcMetrics( Q::Groove ) );

        QskGradient fillFront( t.secondary, QColor( t.secondary ).lighter().toRgb() );
        fillFront.setConicDirection( 0.5, 0.5, 300, 120 );
        ed.setGradient( Q::Fill, fillFront );

        auto fillBack = fillFront;
        fillBack.setConicDirection( 0.5, 0.5, 240, -120 );
        ed.setGradient( Q::Fill | Q::Back, fillBack );

        auto amBack = ed.arcMetrics( Q::Groove | Q::Back );
        amBack.setThickness( 3 );
        ed.setArcMetrics( Q::ProgrammedFill | Q::Back, amBack );

        auto amFront = ed.arcMetrics( Q::Groove );
        amFront.setThickness( 3 );
        ed.setArcMetrics( Q::ProgrammedFill, amFront );

        ed.setGradient( Q::ProgrammedFill, t.onPrimary );
    }

    {
        using Q = QuickAccessButton;

        ed.setStrutSize( Q::Panel, { 166, 112 } );
        ed.setSpacing( Q::Panel, 10 );

        QskGradient g( t.surfaceVariant, t.onSecondary );
        g.setLinearDirection( Qt::Vertical );
        ed.setGradient( Q::Panel, g );

        ed.setBoxShape( Q::Panel, 10 );
        ed.setBoxBorderMetrics( Q::Panel, 1 );
        ed.setBoxBorderColors( Q::Panel, t.surface );

        auto s1 = QskRgb::toTransparentF( t.inverseSurface, 0.1 ); // ### own function
        ed.setShadowColor( Q::Panel, s1 );
        ed.setShadowMetrics( Q::Panel, 1, 4, { 0, 2 } );
        ed.setShadowMetrics( Q::Panel | Q::Hovered, 3, 4, { 0, 2 } );

        ed.setStrutSize( Q::Icon, { 40, 40 } );
        ed.setGraphicRole( Q::Icon, GraphicRoleOnPrimaryContainer );
        ed.setGraphicRole( Q::Icon | Q::Pressed, GraphicRolePrimary );

        ed.setFontRole( Q::Text, { QskFontRole::Body } );
        ed.setColor( Q::Text, t.onPrimaryContainer );
        ed.setColor( Q::Text | Q::Pressed, t.primary );
    }

    {
        using Q = SliderLabel;

        ed.setAlignment( Q::Text, Qt::AlignRight | Qt::AlignVCenter );
        ed.setFontRole( Q::Text, QskFontRole::Title );
    }

    {
        using Q = Speedometer;

        ed.setStrutSize( Q::Panel1, { 160, 160 } );
        ed.setBoxShape( Q::Panel1, 100, Qt::RelativeSize );
        ed.setGradient( Q::Panel1, buttonGradient( m_data->colors.secondary ) );
        ed.setShadowMetrics( Q::Panel1, 1, 4, { 0, 4 } );
        ed.setShadowColor( Q::Panel1, QskRgb::toTransparentF( t.onSecondaryContainer, 0.3 ) );

        ed.setStrutSize( Q::Panel2, { 140, 140 } );
        ed.setBoxShape( Q::Panel2, 100, Qt::RelativeSize );
        const auto g = buttonGradient( t.onPrimary, t.primaryContainer );
        ed.setGradient( Q::Panel2, g );

        ed.setStrutSize( Q::Panel3, { 125, 125 } );
        ed.setBoxShape( Q::Panel3, 100, Qt::RelativeSize );
        ed.setGradient( Q::Panel3, g.reversed() );
        auto s = QskRgb::toTransparentF( t.inverseSurface, 0.1 ); // ### own function
        ed.setShadowMetrics( Q::Panel3, 0, 1, { 0, 1 } );
        ed.setShadowColor( Q::Panel3, s );

        ed.setAlignment( Q::ValueText, Qt::AlignCenter );
        ed.setFontRole( Q::ValueText, QskFontRole::Display );

        ed.setAlignment( Q::UnitText, Qt::AlignCenter );
    }

    {
        using Q = SwitchLabel;

        ed.setStrutSize( Q::Graphic, { -1, 46 } );
        ed.setGraphicRole( Q::Graphic, GraphicRolePrimary );
    }

    {
        using Q = Tile;

        ed.setBoxShape( Q::Panel, 20 );
        QskGradient g( t.surfaceVariant, t.onSecondary );
        g.setLinearDirection( Qt::Vertical );
        ed.setGradient( Q::Panel, g );

        ed.setShadowMetrics( Q::Panel, 2, 4, { 0, 2 } );
        auto s1 = QskRgb::toTransparentF( t.inverseSurface, 0.1 ); // ### own function
        ed.setShadowColor( Q::Panel, s1 );
    }

    {
        using Q = TileLabel;

        ed.setBoxShape( Q::Panel, { 0, 0, 20, 20 } );
        ed.setPadding( Q::Panel, 0, 5, 0, 5 );

        QskGradient g( t.primary, t.onSurfaceVariant );
        g.setLinearDirection( Qt::Vertical );
        ed.setGradient( Q::Panel, g );

        ed.setFontRole( Q::Text, { QskFontRole::Title } );
        ed.setColor( Q::Text, t.onPrimary );
        ed.setAlignment( Q::Text, Qt::AlignCenter );
    }

    {
        using Q = TractorLabel;

        ed.setStrutSize( Q::Graphic, { 100, 100 } );
        ed.setGraphicRole( Q::Graphic, GraphicRoleInversePrimary );
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
        using Q = QskSlider;

        ed.setMetric( Q::Groove | A::Size, 10 );
        ed.setBoxShape( Q::Groove, 100, Qt::RelativeSize );
        ed.setGradient( Q::Groove, sliderGradient( t.primaryContainer, t.onPrimary ) );

        ed.setShadowMetrics( Q::Groove, 1, 2, { 0, 1 } );
        auto s1 = QskRgb::toTransparentF( t.inverseSurface, 0.1 ); // ### own function
        ed.setShadowColor( Q::Groove, s1 );

        ed.setMetric( Q::Fill | A::Size, 10 );
        ed.setBoxShape( Q::Fill, 100, Qt::RelativeSize );
        ed.setGradient( Q::Fill, sliderGradient( t.secondary ) );

        ed.setStrutSize( Q::Handle, { 30, 30 } );
        ed.setBoxShape( Q::Handle, 100, Qt::RelativeSize );
        const auto g = buttonGradient( t.primaryContainer, t.onPrimary );
        ed.setGradient( Q::Handle, g.reversed() );
        ed.setGradient( Q::Handle | Q::Pressed, g );

        auto c = QskRgb::toTransparentF( t.inverseSurface, 0.25 );
        ed.setShadowColor( Q::Handle, c );
        ed.setShadowMetrics( Q::Handle, 2, 4, { 0, 2 } );
    }

    {
        using Q = QskSwitchButton;

        ed.setStrutSize( Q::Groove, { 50, 18 } );
        ed.setBoxShape( Q::Groove, 100, Qt::RelativeSize );
        ed.setGradient( Q::Groove, sliderGradient( t.primaryContainer, t.onPrimary ) );
        ed.setGradient( Q::Groove | Q::Checked, sliderGradient( t.secondary ) );

        ed.setShadowMetrics( Q::Groove, 1, 2, { 0, 1 } );
        auto s1 = QskRgb::toTransparentF( t.inverseSurface, 0.1 ); // ### own function
        ed.setShadowColor( Q::Groove, s1 );

        ed.setStrutSize( Q::Handle, { 30, 30 } );
        ed.setBoxShape( Q::Handle, 100, Qt::RelativeSize );
        ed.setPosition( Q::Handle, 0.0 );
        ed.setPosition( Q::Handle | Q::Checked, 1.0 );
        const auto g = buttonGradient( t.primaryContainer, t.onPrimary );
        ed.setGradient( Q::Handle, g.reversed() );
        ed.setGradient( Q::Handle | Q::Pressed, g );

        auto c = QskRgb::toTransparentF( t.inverseSurface, 0.25 );
        ed.setShadowColor( Q::Handle, c );
        ed.setShadowMetrics( Q::Handle, 2, 4, { 0, 2 } );
    }

    {
        using Q = QskTextLabel;

        ed.setFontRole( Q::Text, { QskFontRole::Body } );
        ed.setColor( Q::Text, t.onPrimaryContainer );
    }
}

void Skin::setupFonts()
{
    using F = QskFontRole;

    setFont( { F::Body, F::Normal }, createFont( 16, QFont::Medium ) );
    setFont( { F::Title, F::Normal }, createFont( 24, QFont::Medium ) );
    setFont( { F::Display, F::Normal }, createFont( 40, QFont::Medium ) );

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
    setGraphicColor( GraphicRoleInversePrimary, theme.inversePrimary );
    setGraphicColor( GraphicRoleOnPrimaryContainer, theme.onPrimaryContainer );
    setGraphicColor( GraphicRoleOnSurfaceVariant, theme.onSurfaceVariant );
    setGraphicColor( GraphicRolePrimary, theme.primary );
}

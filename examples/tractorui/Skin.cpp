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

#include <QskArcMetrics.h>
#include <QskBoxBorderColors.h>
#include <QskColorFilter.h>
#include <QskFontRole.h>
#include <QskHctColor.h>
#include <QskMargins.h>
#include <QskGradientDirection.h>
#include <QskRgbValue.h>
#include <QskSeparator.h>
#include <QskShadowMetrics.h>
#include <QskSlider.h>
#include <QskSkinHintTableEditor.h>
#include <QskSkinManager.h>
#include <QskSwitchButton.h>

#include <QFont>
#include <QFontInfo>

class TractorTheme : public QskMaterial3Theme
{
  public:
    TractorTheme( QskSkin::ColorScheme colorScheme, const BaseColors& baseColors )
        : QskMaterial3Theme( colorScheme, baseColors )
    {
        const QskHctColor primaryBaseHct = QskHctColor( baseColors.primary );
        const QskHctColor secondaryBaseHct = QskHctColor( baseColors.secondary );

        backgroundBase = primaryBaseHct;
        backgroundBase.setChroma( 0 );

        surfaceFlashy = primaryBaseHct;
        surfaceFlashy.setChroma( 40 );

        auto chrome = primaryBaseHct;
        chrome.setChroma( 10 );

        shadow = QskRgb::toTransparentF( inverseSurface, 0.1 );

        if ( colorScheme == QskSkin::LightScheme )
        {
            primaryBase = baseColors.primary;
            primaryBackground = primaryBaseHct.toned( 99 ).rgb();

            secondaryBase = baseColors.secondary;

            chrome1 = chrome.toned( 60 ).rgb();
            chrome2 = chrome.toned( 80 ).rgb();
            chromeInnerBorder = chrome.toned( 100 ).rgb();
            chromeOuterBorder = chrome.toned( 60 ).rgb();

            background1 = backgroundBase.toned( 95 ).rgb();
            background2 = backgroundBase.toned( 99 ).rgb();

            surfaceFlashy.setTone( 40 );
        }
        else
        {
            auto primaryBaseDark = primaryBaseHct;
            primaryBaseDark.setTone( 90 );
            primaryBase = primaryBaseDark.rgb();

            primaryBackground = primaryBaseDark.toned( 15 ).rgb();

            auto secondaryBaseDark = QskHctColor( baseColors.secondary );
            secondaryBaseDark.setTone( 60 );
            secondaryBase = secondaryBaseDark.rgb();

            chrome1 = chrome.toned( 10 ).rgb();
            chrome2 = chrome.toned( 30 ).rgb();
            chromeOuterBorder = chrome.toned( 40 ).rgb();

            background1 = backgroundBase.toned( 10 ).rgb();
            background2 = backgroundBase.toned( 20 ).rgb();

            surfaceFlashy.setTone( 80 );
        }
    }

    QRgb primaryBase;
    QRgb primaryBackground;
    QRgb secondaryBase;
    QRgb chrome1;
    QRgb chrome2;
    QRgb chromeInnerBorder;
    QRgb chromeOuterBorder;

    QskHctColor backgroundBase;
    QRgb background1;
    QRgb background2;

    QskHctColor surfaceFlashy;

    QRgb shadow;
};

namespace
{
    QFont createFont( qreal pixelSize, QFont::Weight weight )
    {
        QFont font( QStringLiteral( "Calistoga" ), -1, weight );
        font.setPixelSize( pixelSize );

        return font;
    }

    QskGradient backgroundGradient( const TractorTheme& theme )
    {
        QskGradient g( {
            { 0.0, theme.background1 },
            { 0.5, theme.background2 },
            { 1.0, theme.background1 },
        } );
        g.setLinearDirection( Qt::Horizontal );

        return g;
    }

    QskGradient buttonGradient( QRgb rgb )
    {
        QskHctColor c( rgb );

        const auto t = c.tone();
        QskGradient g( c.toned( t + 10 ).rgb(), c.toned( t - 10 ).rgb() );

        g.setLinearDirection( 0, 0, 1, 1 );

        return g;
    }

    QskGradient radialGradient( QRgb rgb )
    {
        QskHctColor c( rgb );

        const auto t = c.tone();
        QskGradient g( c.toned( t + 10 ).rgb(), c.toned( t - 10 ).rgb() );

        g.setRadialDirection( {} );

        return g;
    }

    QskGradient conicalGradient( QRgb rgb )
    {
        QskHctColor c( rgb );

        const auto t = c.tone();
        QskGradient g( c.toned( t - 10 ).rgb(), c.toned( t + 10 ).rgb() );

        g.setConicDirection( {} );

        return g;
    }

    QskGradient verticalGradient( QRgb rgb )
    {

        QskHctColor c( rgb );

        const auto t = c.tone();
        QskGradient g( c.toned( t - 10 ).rgb(), c.toned( t + 10 ).rgb() );

        if( qskSkinManager->currentSkin()->colorScheme() == QskSkin::DarkScheme )
        {
            g = g.reversed();
        }

        g.setLinearDirection( Qt::Vertical );

        return g;
    }
}

class Skin::PrivateData
{
  public:
    PrivateData( const QskMaterial3Theme::BaseColors& baseColors )
        : colors( baseColors )
    {
    }

    QskMaterial3Theme::BaseColors colors;
};

Skin::Skin( QObject* parent )
    : Skin( { 0xff002b54, 0xff5cc061, 0xff98a6ff }, parent )
{
}

Skin::Skin( const QskMaterial3Theme::BaseColors& baseColors, QObject* parent )
    : QskSkin( parent )
    , m_data( new PrivateData( baseColors ) )
{
    declareSkinlet< Button, ButtonSkinlet >();
    declareSkinlet< PowerLiftArc, PowerLiftArcSkinlet >();
    declareSkinlet< Speedometer, SpeedometerSkinlet >();

    setColorScheme( LightScheme );
}

Skin::~Skin()
{
}

void Skin::setBaseColors( const QskMaterial3Theme::BaseColors& colors )
{
    m_data->colors = colors;

    initHints();
}

void Skin::initHints()
{
    setupFonts();

    const TractorTheme theme( colorScheme(), m_data->colors );

    setupGraphicFilters( theme );

    QskSkinHintTableEditor ed( &hintTable() );

    using A = QskAspect;

    {
        using Q = Button;

        // normal buttons:

        ed.setStrutSize( Q::Panel, { 75, 75 } );
        ed.setBoxShape( Q::Panel, 100, Qt::RelativeSize );
        ed.setGradient( Q::Panel, buttonGradient( theme.secondary ) );
        ed.setShadowColor( Q::Panel, QskRgb::toTransparentF( theme.secondary, 0.3 ) );
        ed.setShadowMetrics( Q::Panel, 0, 4, { 0, 4 } );
        ed.setShadowMetrics( Q::Panel | Q::Hovered, 2, 4, { 0, 4 } );
        ed.setSpacing( Q::Panel, 5 );

        ed.setStrutSize( Q::InnerPanel1, { 72, 72 } );
        ed.setBoxShape( Q::InnerPanel1, 100, Qt::RelativeSize );

        const auto g = buttonGradient( theme.primaryBackground );
        ed.setGradient( Q::InnerPanel1, g );
        ed.setGradient( Q::InnerPanel1 | Q::Pressed, g.reversed() );

        ed.setStrutSize( Q::InnerPanel2, { 67, 67 } );
        ed.setBoxShape( Q::InnerPanel2, 100, Qt::RelativeSize );
        ed.setGradient( Q::InnerPanel2, g.reversed() );
        ed.setGradient( Q::InnerPanel2 | Q::Pressed, g );

        ed.setStrutSize( Q::Icon, { 35, 35 } );
        ed.setGraphicRole( Q::Icon, GraphicRolePrimaryBase );
        ed.setGraphicRole( Q::Icon | Q::Pressed, GraphicRoleOnPrimaryContainer );

        ed.setFontRole( Q::Text, { QskFontRole::Body } );
        ed.setColor( Q::Text, theme.onPrimaryContainer );


        // sidebar buttons:

        const auto s = Q::Sidebar;

        ed.setStrutSize( Q::Panel | s, { 85, 85 } );
        ed.setBoxShape( Q::Panel | s, 100, Qt::RelativeSize );

        const auto sg = buttonGradient( theme.primaryBackground );

        ed.setGradient( Q::Panel | s, sg );
        ed.setGradient( Q::Panel | s | Q::Pressed, sg.reversed() );
        ed.setShadowColor( Q::Panel | s, QskRgb::toTransparentF( theme.inverseSurface, 0.2 ) );
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

        const auto g2 = QskRgb::interpolated( theme.background1, theme.background2, 0.5 );
        QskGradient g( theme.background1, g2 );
        g.setLinearDirection( Qt::Horizontal );

        ed.setGradient( Q::Panel | A::Left, g );
        ed.setGradient( R::Panel, g ); // ### left and right
        ed.setGradient( Q::Panel | A::Right, g.reversed() );

        ed.setShadowColor( Q::Panel, theme.shadow );
        ed.setShadowMetrics( Q::Panel, 1, 3, { 1, 1 } );

        auto s2 = QskRgb::toTransparentF( theme.surface, 0.6 );
        ed.setShadowColor( R::Panel, s2 );
        ed.setShadowMetrics( R::Panel, 1, 3, { -1, -1 } );
    }

    {
        using Q = MainBox;

        ed.setGradient( Q::Panel, backgroundGradient( theme ) );
    }

    {
        using Q = PowerLiftArc;

        ed.setArcMetrics( Q::Groove | Q::Back, { -120, -120, 15 } );
        ed.setArcMetrics( Q::Groove, { -60, 120, 15 } );
        QskGradient groove( { { 0.0, theme.primaryContainer }, { 0.4, theme.primaryContainer },
            { 0.5, theme.onPrimary }, { 1.0, theme.onPrimary } } );
        groove.setRadialDirection( 0.5, 0.5, 1 );
        ed.setGradient( Q::Groove, groove );

        ed.setShadowColor( Q::Groove, theme.shadow );
        ed.setShadowMetrics( Q::Groove, 0, 5, { 0, 0 } );

        ed.setArcMetrics( Q::Fill, ed.arcMetrics( Q::Groove ) );

        QskHctColor c( theme.secondaryBase );
        QskGradient fillFront( c.toned( c.tone() - 5 ).rgb(), c.toned( c.tone() + 5 ).rgb() );
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

        ed.setGradient( Q::ProgrammedFill, theme.onPrimary );
    }

    {
        using Q = QuickAccessButton;

        ed.setStrutSize( Q::Panel, { 166, 112 } );
        ed.setSpacing( Q::Panel, 10 );

        QskGradient g( theme.surfaceVariant, theme.onSecondary );
        g.setLinearDirection( Qt::Vertical );
        ed.setGradient( Q::Panel, verticalGradient( theme.primaryBackground ) );

        ed.setBoxShape( Q::Panel, 10 );
        ed.setBoxBorderMetrics( Q::Panel, 1 );
        ed.setBoxBorderColors( Q::Panel, theme.surface );

        ed.setShadowColor( Q::Panel, theme.shadow );
        ed.setShadowMetrics( Q::Panel, 1, 4, { 0, 2 } );
        ed.setShadowMetrics( Q::Panel | Q::Hovered, 3, 4, { 0, 2 } );

        ed.setStrutSize( Q::Icon, { 40, 40 } );
        ed.setGraphicRole( Q::Icon, GraphicRoleOnPrimaryContainer );
        ed.setGraphicRole( Q::Icon | Q::Pressed, GraphicRolePrimary );

        ed.setFontRole( Q::Text, { QskFontRole::Body } );
        ed.setColor( Q::Text, theme.onPrimaryContainer );
        ed.setColor( Q::Text | Q::Pressed, theme.primary );
    }

    {
        using Q = SliderLabel;

        ed.setAlignment( Q::Text, Qt::AlignRight | Qt::AlignVCenter );
        ed.setFontRole( Q::Text, QskFontRole::Title );
        ed.setColor( Q::Text, theme.onPrimaryContainer );
    }

    {
        using Q = Speedometer;

        ed.setStrutSize( Q::OuterPanel, { 170, 170 } );
        ed.setBoxShape( Q::OuterPanel, 100, Qt::RelativeSize );

        QskGradient chromeGradient( {
            { 0.0, theme.chrome1 },
            { 0.1, theme.chrome2 },
            { 0.2, theme.chrome1 },
            { 0.3, theme.chrome2 },
            { 0.4, theme.chrome1 },
            { 0.5, theme.chrome2 },
            { 0.6, theme.chrome1 },
            { 0.7, theme.chrome2 },
            { 0.8, theme.chrome1 },
            { 0.9, theme.chrome2 },
            { 1.0, theme.chrome1 },
            } );
        chromeGradient.setDirection( QskGradient::Conic );

        ed.setGradient( Q::OuterPanel, chromeGradient );
        ed.setShadowMetrics( Q::OuterPanel, 1, 2, { 0, 2 } );
        ed.setShadowColor( Q::OuterPanel, QskRgb::toTransparentF( theme.onSecondaryContainer, 0.3 ) );
        ed.setBoxBorderMetrics( Q::OuterPanel, 1 );
        ed.setBoxBorderColors( Q::OuterPanel, theme.chromeOuterBorder );

        ed.setStrutSize( Q::MiddlePanel, { 150, 150 } );
        ed.setBoxShape( Q::MiddlePanel, 100, Qt::RelativeSize );
        const auto g = buttonGradient( theme.primaryBackground );
        ed.setGradient( Q::MiddlePanel, g );
        ed.setBoxBorderMetrics( Q::MiddlePanel, 1 );
        ed.setBoxBorderColors( Q::MiddlePanel, theme.chromeInnerBorder );

        ed.setMetric( Q::Tickmarks, 1 );
        ed.setArcMetrics( Q::Tickmarks, { 225, -270, 15 } );
        ed.setColor( Q::Tickmarks, theme.chromeOuterBorder );

        ed.setStrutSize( Q::InnerPanel, { 143, 143 } );
        ed.setBoxShape( Q::InnerPanel, 100, Qt::RelativeSize );
        ed.setGradient( Q::InnerPanel, g );
        ed.setShadowMetrics( Q::InnerPanel, 3, 3, { 0, 0 } );
        ed.setShadowColor( Q::InnerPanel, theme.shadow );

        ed.setArcMetrics( Q::Value, { 225, -270, 11 } );
        auto valueGradient = conicalGradient( theme.secondaryBase );
        valueGradient.setConicDirection( 0.5, 0.5, 225, -270 );
        ed.setGradient( Q::Value, valueGradient );
        ed.setShadowColor( Q::Value, QskRgb::toTransparentF( theme.secondaryBase, 0.3 ) );
        ed.setShadowMetrics( Q::Value, 0, 3, { 1, 1 } );

        ed.setStrutSize( Q::Intensity, { 120, 120 } );
        ed.setArcMetrics( Q::Intensity, { 225, -270, 5 } );
        QskGradient intensityGradient( QColor::fromRgba( 0x3300ff00 ), QColor::fromRgba( 0x33ff0000 ) );
        intensityGradient.setConicDirection( 0.5, 0.5, 225, -270 );
        ed.setGradient( Q::Intensity, intensityGradient );

        ed.setAlignment( Q::ValueText, Qt::AlignCenter );
        ed.setFontRole( Q::ValueText, QskFontRole::Display );
        ed.setColor( Q::ValueText, theme.onPrimaryContainer );

        ed.setAlignment( Q::UnitText, Qt::AlignCenter );
        ed.setColor( Q::UnitText, theme.onPrimaryContainer );
    }

    {
        using Q = SwitchLabel;

        ed.setStrutSize( Q::Graphic, { -1, 46 } );
        ed.setGraphicRole( Q::Graphic, GraphicRolePrimaryBase );
    }

    {
        using Q = Tile;

        ed.setBoxShape( Q::Panel, 20 );
        QskGradient g( theme.background1, theme.background2 );
        g.setLinearDirection( Qt::Vertical );
        ed.setGradient( Q::Panel, g );

        ed.setShadowMetrics( Q::Panel, 2, 4, { 0, 2 } );
        ed.setShadowColor( Q::Panel, theme.shadow );
    }

    {
        using Q = TileLabel;

        ed.setBoxShape( Q::Panel, { 0, 0, 20, 20 } );
        ed.setPadding( Q::Panel, 0, 5, 0, 5 );

        ed.setGradient( Q::Panel, theme.surfaceFlashy.rgb() );

        ed.setFontRole( Q::Text, { QskFontRole::Title } );
        ed.setColor( Q::Text, theme.onPrimary );
        ed.setAlignment( Q::Text, Qt::AlignCenter );
    }

    {
        using Q = TractorLabel;

        ed.setStrutSize( Q::Graphic, { 100, 100 } );
        ed.setGraphicRole( Q::Graphic, GraphicRoleInversePrimary );
    }

    {
        using Q = QskGraphicLabel;

        ed.setGraphicRole( Q::Graphic, GraphicRolePrimaryBase );
    }

    {
        using Q = QskSeparator;

        ed.setMetric( Q::Panel | A::Size, 1 );
        ed.setGradient( Q::Panel, theme.surface );
    }

    {
        using Q = QskSlider;

        ed.setMetric( Q::Groove | A::Size, 10 );
        ed.setBoxShape( Q::Groove, 100, Qt::RelativeSize );
        ed.setGradient( Q::Groove, verticalGradient( theme.primaryBackground ) );

        ed.setShadowMetrics( Q::Groove, 1, 2, { 0, 1 } );
        ed.setShadowColor( Q::Groove, theme.shadow );

        ed.setMetric( Q::Fill | A::Size, 10 );
        ed.setBoxShape( Q::Fill, 100, Qt::RelativeSize );
        ed.setGradient( Q::Fill, theme.secondaryBase );
        ed.setBoxBorderMetrics( Q::Fill, 1 );
        ed.setBoxBorderColors( Q::Fill, QskRgb::toTransparentF( theme.secondary, 0.2 ) );

        ed.setStrutSize( Q::Handle, { 30, 30 } );
        ed.setBoxShape( Q::Handle, 100, Qt::RelativeSize );
        const auto g = buttonGradient( theme.onPrimary );
        ed.setGradient( Q::Handle, g );
        ed.setGradient( Q::Handle | Q::Pressed, g.reversed() );

        auto c = QskRgb::toTransparentF( theme.inverseSurface, 0.25 );
        ed.setShadowColor( Q::Handle, c );
        ed.setShadowMetrics( Q::Handle, 1, 2, { 0, 1 } );
    }

    {
        using Q = QskSwitchButton;

        ed.setStrutSize( Q::Groove, { 50, 18 } );
        ed.setBoxShape( Q::Groove, 100, Qt::RelativeSize );
        ed.setGradient( Q::Groove, verticalGradient( theme.primaryBackground ) );
        ed.setGradient( Q::Groove | Q::Checked, verticalGradient( theme.secondaryBase ) );

        ed.setShadowMetrics( Q::Groove, 1, 2, { 0, 1 } );
        ed.setShadowColor( Q::Groove, theme.shadow );

        ed.setStrutSize( Q::Handle, { 30, 30 } );
        ed.setBoxShape( Q::Handle, 100, Qt::RelativeSize );
        ed.setPosition( Q::Handle, 0.0 );
        ed.setPosition( Q::Handle | Q::Checked, 1.0 );
        const auto g = buttonGradient( theme.onPrimary );
        ed.setGradient( Q::Handle, g );
        ed.setGradient( Q::Handle | Q::Pressed, g.reversed() );

        auto c = QskRgb::toTransparentF( theme.inverseSurface, 0.25 );
        ed.setShadowColor( Q::Handle, c );
        ed.setShadowMetrics( Q::Handle, 1, 2, { 0, 1 } );
    }

    {
        using Q = QskTextLabel;

        ed.setFontRole( Q::Text, { QskFontRole::Body } );
        ed.setColor( Q::Text, theme.onPrimaryContainer );
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

void Skin::setupGraphicFilters( const TractorTheme& theme )
{
    setGraphicColor( GraphicRoleInversePrimary, theme.inversePrimary );
    setGraphicColor( GraphicRoleOnPrimaryContainer, theme.onPrimaryContainer );
    setGraphicColor( GraphicRoleOnSurfaceVariant, theme.onSurfaceVariant );
    setGraphicColor( GraphicRolePrimary, theme.primary );
    setGraphicColor( GraphicRolePrimaryBase, theme.primaryBase );
}

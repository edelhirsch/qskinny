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
#include <QskRgbValue.h>
#include <QskSeparator.h>
#include <QskShadowMetrics.h>
#include <QskSlider.h>
#include <QskSkinHintTableEditor.h>
#include <QskSwitchButton.h>

#include <QFont>
#include <QFontInfo>

class TractorTheme : public QskMaterial3Theme
{
  public:
    TractorTheme( QskSkin::ColorScheme colorScheme, const BaseColors& baseColors )
        : QskMaterial3Theme( colorScheme, baseColors )
    {
        QskHctColor primaryHct = QskHctColor( baseColors.primary );

        backgroundBase = primaryHct;
        backgroundBase.setChroma( 0 );

        surfaceFlashy = primaryHct;
        surfaceFlashy.setChroma( 30 );

        if ( colorScheme == QskSkin::LightScheme )
        {
            primaryBase = baseColors.primary;

            background1 = backgroundBase.toned( 95 ).rgb();
            background2 = backgroundBase.toned( 99 ).rgb();

            surfaceFlashy.setTone( 40 );
        }
        else
        {
            primaryBase = baseColors.primary;

            background1 = backgroundBase.toned( 10 ).rgb();
            background2 = backgroundBase.toned( 20 ).rgb();

            surfaceFlashy.setTone( 80 );
        }
    }

    QRgb primaryBase;

    QskHctColor backgroundBase;
    QRgb background1;
    QRgb background2;

    QskHctColor surfaceFlashy;
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

        const auto g = buttonGradient( theme.primaryContainer, theme.onPrimary );
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

        QskGradient sg( theme.onPrimary, theme.surfaceVariant );
        sg.setLinearDirection( 0, 0, 1, 1 );

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

        auto s1 = QskRgb::toTransparentF( theme.inverseSurface, 0.1 ); // ### own function
        ed.setShadowColor( Q::Panel, s1 );
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

        auto s1 = QskRgb::toTransparentF( theme.inverseSurface, 0.1 ); // ### own function
        ed.setShadowColor( Q::Groove, s1 );
        ed.setShadowMetrics( Q::Groove, 0, 5, { 0, 0 } );

        ed.setArcMetrics( Q::Fill, ed.arcMetrics( Q::Groove ) );

        QskGradient fillFront( theme.secondary, QColor( theme.secondary ).lighter().toRgb() );
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
        ed.setGradient( Q::Panel, g );

        ed.setBoxShape( Q::Panel, 10 );
        ed.setBoxBorderMetrics( Q::Panel, 1 );
        ed.setBoxBorderColors( Q::Panel, theme.surface );

        auto s1 = QskRgb::toTransparentF( theme.inverseSurface, 0.1 ); // ### own function
        ed.setShadowColor( Q::Panel, s1 );
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
    }

    {
        using Q = Speedometer;

        ed.setStrutSize( Q::Panel1, { 160, 160 } );
        ed.setBoxShape( Q::Panel1, 100, Qt::RelativeSize );
        ed.setGradient( Q::Panel1, buttonGradient( m_data->colors.secondary ) );
        ed.setShadowMetrics( Q::Panel1, 1, 4, { 0, 4 } );
        ed.setShadowColor( Q::Panel1, QskRgb::toTransparentF( theme.onSecondaryContainer, 0.3 ) );

        ed.setStrutSize( Q::Panel2, { 140, 140 } );
        ed.setBoxShape( Q::Panel2, 100, Qt::RelativeSize );
        const auto g = buttonGradient( theme.onPrimary, theme.primaryContainer );
        ed.setGradient( Q::Panel2, g );

        ed.setStrutSize( Q::Panel3, { 125, 125 } );
        ed.setBoxShape( Q::Panel3, 100, Qt::RelativeSize );
        ed.setGradient( Q::Panel3, g.reversed() );
        auto s = QskRgb::toTransparentF( theme.inverseSurface, 0.1 ); // ### own function
        ed.setShadowMetrics( Q::Panel3, 0, 1, { 0, 1 } );
        ed.setShadowColor( Q::Panel3, s );

        ed.setAlignment( Q::ValueText, Qt::AlignCenter );
        ed.setFontRole( Q::ValueText, QskFontRole::Display );

        ed.setAlignment( Q::UnitText, Qt::AlignCenter );
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
        auto s1 = QskRgb::toTransparentF( theme.inverseSurface, 0.1 ); // ### own function
        ed.setShadowColor( Q::Panel, s1 );
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
        ed.setGradient( Q::Groove, sliderGradient( theme.primaryContainer, theme.onPrimary ) );

        ed.setShadowMetrics( Q::Groove, 1, 2, { 0, 1 } );
        auto s1 = QskRgb::toTransparentF( theme.inverseSurface, 0.1 ); // ### own function
        ed.setShadowColor( Q::Groove, s1 );

        ed.setMetric( Q::Fill | A::Size, 10 );
        ed.setBoxShape( Q::Fill, 100, Qt::RelativeSize );
        ed.setGradient( Q::Fill, sliderGradient( theme.secondary ) );

        ed.setStrutSize( Q::Handle, { 30, 30 } );
        ed.setBoxShape( Q::Handle, 100, Qt::RelativeSize );
        const auto g = buttonGradient( theme.primaryContainer, theme.onPrimary );
        ed.setGradient( Q::Handle, g.reversed() );
        ed.setGradient( Q::Handle | Q::Pressed, g );

        auto c = QskRgb::toTransparentF( theme.inverseSurface, 0.25 );
        ed.setShadowColor( Q::Handle, c );
        ed.setShadowMetrics( Q::Handle, 2, 4, { 0, 2 } );
    }

    {
        using Q = QskSwitchButton;

        ed.setStrutSize( Q::Groove, { 50, 18 } );
        ed.setBoxShape( Q::Groove, 100, Qt::RelativeSize );
        ed.setGradient( Q::Groove, sliderGradient( theme.primaryContainer, theme.onPrimary ) );
        ed.setGradient( Q::Groove | Q::Checked, sliderGradient( theme.secondary ) );

        ed.setShadowMetrics( Q::Groove, 1, 2, { 0, 1 } );
        auto s1 = QskRgb::toTransparentF( theme.inverseSurface, 0.1 ); // ### own function
        ed.setShadowColor( Q::Groove, s1 );

        ed.setStrutSize( Q::Handle, { 30, 30 } );
        ed.setBoxShape( Q::Handle, 100, Qt::RelativeSize );
        ed.setPosition( Q::Handle, 0.0 );
        ed.setPosition( Q::Handle | Q::Checked, 1.0 );
        const auto g = buttonGradient( theme.primaryContainer, theme.onPrimary );
        ed.setGradient( Q::Handle, g.reversed() );
        ed.setGradient( Q::Handle | Q::Pressed, g );

        auto c = QskRgb::toTransparentF( theme.inverseSurface, 0.25 );
        ed.setShadowColor( Q::Handle, c );
        ed.setShadowMetrics( Q::Handle, 2, 4, { 0, 2 } );
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

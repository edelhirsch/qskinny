/******************************************************************************
 * Copyright (C) The authors
 *           SPDX-License-Identifier: BSD-3-Clause
 *****************************************************************************/

#include "MainBox.h"

#include "Button.h"
#include "Switch.h"

#include <QskGraphicLabel.h>
#include <QskGridBox.h>
#include <QskLinearBox.h>
#include <QskSeparator.h>
#include <QskTextLabel.h>

QSK_SUBCONTROL( HeaderElementsBackgroundBox, Panel )

// just for displaying shadow in another color and avoiding a skinlet:
HeaderElementsBackgroundBox::HeaderElementsBackgroundBox( QQuickItem* parent )
    : QskBox( parent )
{
    setSubcontrolProxy( QskBox::Panel, Panel );
}


QSK_SUBCONTROL( HeaderElementsBox, Panel )

class HeaderElementsBox::PrivateData
{
  public:
    Position pos;
    HeaderElementsBackgroundBox* backgroundBox;
};

HeaderElementsBox::HeaderElementsBox( HeaderElementsBox::Position pos, QQuickItem* parent )
    : QskLinearBox( Qt::Horizontal, parent )
    , m_data( new PrivateData )
{
    setPanel( true );
    setPolishOnResize( true );

    setSpacing( 30 ); // ### style

    setSubcontrolProxy( QskLinearBox::Panel, Panel );

    m_data->pos = pos;
    m_data->backgroundBox = new HeaderElementsBackgroundBox( this );
}

void HeaderElementsBox::updateLayout()
{
    QskLinearBox::updateLayout();

    m_data->backgroundBox->setGradientHint( QskBox::Panel, gradientHint( Panel ) );
    m_data->backgroundBox->setGeometry( { 0, 0 }, size() );
}

QskAspect::Variation HeaderElementsBox::effectiveVariation() const
{
    switch( m_data->pos )
    {
        case Position::Left:
            return QskAspect::Left;
        default:
            return QskAspect::Right;
    }
}


QSK_SUBCONTROL( MainBox, Panel )

class MainBox::PrivateData
{
  public:
    QskLinearBox* headerBox;
    HeaderElementsBox* leftElements;
    HeaderElementsBox* rightElements;

    QskLinearBox* contentBox;
    QskLinearBox* sidebarBox;
    QskGridBox* tileArea;
};

MainBox::MainBox( QQuickItem* parent )
    : QskLinearBox( Qt::Vertical, parent )
    , m_data( new PrivateData )
{
    setPanel( true );

    setSubcontrolProxy( QskLinearBox::Panel, Panel );

    setPadding( 20 ); // ### style?
    setSpacing( 15 );

    setupHeaderBox();
    setupContentBox();
}

void MainBox::setupHeaderBox()
{
    m_data->headerBox = new QskLinearBox( Qt::Horizontal, this );
    m_data->headerBox->setSpacing( 20 );

    m_data->leftElements = new HeaderElementsBox( HeaderElementsBox::Position::Left, m_data->headerBox );
    m_data->leftElements->setPadding( { 0, 10, 25, 10 } );

    auto* logo = new QskGraphicLabel( "logo", m_data->headerBox );
    logo->setAlignment( Qt::AlignCenter );
    logo->setMargins( 15 );

    m_data->rightElements = new HeaderElementsBox( HeaderElementsBox::Position::Right, m_data->headerBox );
    m_data->rightElements->setPadding( m_data->leftElements->padding() );


    auto* date = new QskTextLabel( m_data->leftElements );
    date->setSizePolicy( Qt::Horizontal, QskSizePolicy::Fixed );
    date->setText( "day\ndate\ntime");

    new QskSeparator( Qt::Vertical, m_data->leftElements );

    auto* bluetoothSwitch = new Switch( "bluetooth", m_data->leftElements );

    new QskSeparator( Qt::Vertical, m_data->leftElements );

    auto* userButton = new Button( "user", m_data->leftElements );
    userButton->setText( "Hans" );


    auto* wifiSwitch = new Switch( "wifi", m_data->rightElements );

    new QskSeparator( Qt::Vertical, m_data->rightElements );

    auto* mapsSwitch = new Switch( "map-location", m_data->rightElements );

    new QskSeparator( Qt::Vertical, m_data->rightElements );

    auto* settingsButton = new Button( "bars", m_data->rightElements );
    settingsButton->setText( "settings" );
}

void MainBox::setupContentBox()
{
    m_data->contentBox = new QskLinearBox( Qt::Horizontal, this );
    m_data->contentBox->setSizePolicy( Qt::Vertical, QskSizePolicy::Expanding );

    setupSidebar();
    setupTileArea();
}

void MainBox::setupSidebar()
{
    m_data->sidebarBox = new QskLinearBox( Qt::Vertical, m_data->contentBox );

    const auto buttonIcons = { "house", "car-battery", "grip", "square-poll-vertical", "tractor", "road" };

    for( const auto& icon : buttonIcons )
    {
        auto* b = new Button( icon, m_data->sidebarBox );
        b->setType( Button::Type::Sidebar );
    }
}

void MainBox::setupTileArea()
{

}

#include "moc_MainBox.cpp"

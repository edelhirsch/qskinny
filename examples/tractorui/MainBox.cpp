/******************************************************************************
 * Copyright (C) The authors
 *           SPDX-License-Identifier: BSD-3-Clause
 *****************************************************************************/

#include "MainBox.h"

#include "Button.h"
#include "CabinTile.h"
#include "EngineTile.h"
#include "PowerLiftTile.h"
#include "QuickAccessTile.h"
#include "Switch.h"

#include <QskGraphicLabel.h>
#include <QskGridBox.h>
#include <QskLinearBox.h>
#include <QskSeparator.h>
#include <QskTextLabel.h>

#include <QLocale>
#include <QTimer>

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

    m_data->backgroundBox->setGeometry( geometry() );
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
    setSpacing( 25 );

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


    auto* dateTimeLabel = new QskTextLabel( m_data->leftElements );
    dateTimeLabel->setSizePolicy( Qt::Horizontal, QskSizePolicy::Fixed );
    dateTimeLabel->setAlignment( Qt::AlignCenter );

    auto updateDateTime = [dateTimeLabel]()
    {
        auto l = QLocale::system();
        auto cdt = QDateTime::currentDateTime();

        auto day = l.dayName( QDate::currentDate().dayOfWeek() );
        auto date = cdt.toString( l.dateFormat( QLocale::ShortFormat ) );
        auto t = cdt.toString( l.timeFormat( QLocale::ShortFormat ) );

        dateTimeLabel->setText( QString( "%1\n%2\n%3").arg( day ).arg( date ).arg( t ) );
    };

    auto* timer = new QTimer( this );
    connect( timer, &QTimer::timeout, this, updateDateTime );
    timer->start( 1000 * 60 );

    updateDateTime();

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
    m_data->contentBox->setSpacing( 20 );

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
    m_data->tileArea = new QskGridBox( m_data->contentBox );
    m_data->tileArea->setSizePolicy( QskSizePolicy::Expanding, QskSizePolicy::Expanding );
    m_data->tileArea->setSpacing( 20 );

    auto* engineTile = new EngineTile( m_data->tileArea );
    m_data->tileArea->addItem( engineTile, 0, 0 );

    auto* quickAccessTile = new QuickAccessTile( m_data->tileArea );
    m_data->tileArea->addItem( quickAccessTile, 0, 1 );

    auto* powerLiftTile = new PowerLiftTile( m_data->tileArea );
    m_data->tileArea->addItem( powerLiftTile, 1, 0 );

    auto* cabinTile = new CabinTile( m_data->tileArea );
    m_data->tileArea->addItem( cabinTile, 1, 1 );
}

#include "moc_MainBox.cpp"

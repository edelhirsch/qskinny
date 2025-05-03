/******************************************************************************
 * Copyright (C) The authors
 *           SPDX-License-Identifier: BSD-3-Clause
 *****************************************************************************/

#include "MainBox.h"

#include "Button.h"
#include "CabinTile.h"
#include "Cube.h"
#include "EngineTile.h"
#include "PowerLiftTile.h"
#include "QuickAccessTile.h"
#include "Switch.h"
#include "ThemeColorsTile.h"

#include <QskGesture.h>
#include <QskEvent.h>
#include <QskGraphicLabel.h>
#include <QskGridBox.h>
#include <QskLinearBox.h>
#include <QskPanGestureRecognizer.h>
#include <QskSeparator.h>
#include <QskTextLabel.h>

#include <QLocale>
#include <QTimer>

namespace
{
    class PanRecognizer final : public QskPanGestureRecognizer
    {
      public:
        PanRecognizer( MainBox* mainItem )
            : QskPanGestureRecognizer( mainItem )
        {
            setOrientations( Qt::Horizontal | Qt::Vertical );
            setMinDistance( 50 );
            setTimeout( 100 );

            setWatchedItem( mainItem );
        }
    };

    class EmptyTile : public Tile
    {
      public:
        EmptyTile( QQuickItem* parent )
            : Tile( "empty", parent )
        {
        }
    };
}

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
    QskGraphicLabel* logo;
    HeaderElementsBox* leftElements;
    HeaderElementsBox* rightElements;

    QskLinearBox* contentBox;
    QskLinearBox* sidebarBox;
    Cube* cube;
    QskGridBox* mainTileArea;
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

void MainBox::gestureEvent( QskGestureEvent* event )
{
    if( event->gesture()->state() == QskGesture::Finished
        && event->gesture()->type() == QskGesture::Pan )
    {
        const auto* panGesture = static_cast< const QskPanGesture* >( event->gesture().get() );

        const auto delta = panGesture->origin() - panGesture->position();

        Qsk::Direction direction;

        if( qAbs( delta.x() ) > qAbs( delta.y() ) )
        {
            direction = ( delta.x() < 0 ) ? Qsk::LeftToRight : Qsk::RightToLeft;
        }
        else
        {
            direction = ( delta.y() < 0 ) ? Qsk::TopToBottom : Qsk::BottomToTop;
        }

        m_data->cube->switchPosition( direction );
    }
}

void MainBox::keyPressEvent( QKeyEvent* event )
{
    // maybe using shortcuts ?

    Qsk::Direction direction;

    switch( event->key() )
    {
        case Qt::Key_Up:
            direction = Qsk::TopToBottom;
            break;

        case Qt::Key_Down:
            direction = Qsk::BottomToTop;
            break;

        case Qt::Key_Left:
            direction = Qsk::LeftToRight;
            break;

        case Qt::Key_Right:
            direction = Qsk::RightToLeft;
            break;

        default:
            return;
    }

    m_data->cube->switchPosition( direction );
}

void MainBox::setupHeaderBox()
{
    m_data->headerBox = new QskLinearBox( Qt::Horizontal, this );
    m_data->headerBox->setSpacing( 20 );

    m_data->leftElements = new HeaderElementsBox( HeaderElementsBox::Position::Left, m_data->headerBox );
    m_data->leftElements->setPadding( { 0, 10, 25, 10 } );

    m_data->logo = new QskGraphicLabel( "logo", m_data->headerBox );
    m_data->logo->setAlignment( Qt::AlignCenter );
    m_data->logo->setMargins( 15 );

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
    setupCube();

    m_data->mainTileArea = new QskGridBox( m_data->cube );

    m_data->mainTileArea->setSpacing( 20 );

    auto* engineTile = new EngineTile( m_data->mainTileArea );
    m_data->mainTileArea->addItem( engineTile, 0, 0 );

    auto* themeColorsTile = new ThemeColorsTile( m_data->mainTileArea );
    m_data->mainTileArea->addItem( themeColorsTile, 0, 1 );

    connect( themeColorsTile, &ThemeColorsTile::logoChanged, this, [this]( const QUrl& url )
    {
        m_data->logo->setSource( url );
    } );

    auto* powerLiftTile = new PowerLiftTile( m_data->mainTileArea );
    m_data->mainTileArea->addItem( powerLiftTile, 1, 0 );

    auto* cabinTile = new CabinTile( m_data->mainTileArea );
    m_data->mainTileArea->addItem( cabinTile, 1, 1 );

    for( int pos = 0; pos < Cube::NumPositions; pos++ )
    {
        if( pos == Cube::FrontPos )
        {
            m_data->cube->insertItem( Cube::FrontPos, m_data->mainTileArea );
        }
        else
        {
            auto* emptyTileArea = new QskGridBox( m_data->cube );

            emptyTileArea->setSpacing( 20 );

            for( int i = 0; i < 4; i++ )
            {
                auto* emptyTile = new EmptyTile( emptyTileArea );

                int row = i / 2, col = i % 2;
                emptyTileArea->addItem( emptyTile, row, col );
            }

            m_data->cube->insertItem( pos, emptyTileArea );
        }
    }

    m_data->cube->setCurrentItem( m_data->mainTileArea );
}

void MainBox::setupCube()
{
    setAcceptedMouseButtons( Qt::LeftButton );
    setFiltersChildMouseEvents( true );

    (void) new PanRecognizer( this );

    m_data->cube = new Cube( m_data->contentBox );
    m_data->cube->setSizePolicy( QskSizePolicy::Expanding, QskSizePolicy::Expanding );
}

#include "moc_MainBox.cpp"

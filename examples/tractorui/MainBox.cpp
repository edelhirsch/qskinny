/******************************************************************************
 * Copyright (C) The authors
 *           SPDX-License-Identifier: BSD-3-Clause
 *****************************************************************************/

#include "MainBox.h"

#include "Button.h"
#include "CabinTile.h"
#include "Cube.h"
#include "EngineTile.h"
#include "FrontPowerLiftTile.h"
#include "QuickAccessTile.h"
#include "Switch.h"
#include "ThemeColorsTile.h"

#include <QskGesture.h>
#include <QskEvent.h>
#include <QskGraphicLabel.h>
#include <QskGridBox.h>
#include <QskSkinManager.h>
#include <QskLinearBox.h>
#include <QskMenu.h>
#include <QskPanGestureRecognizer.h>
#include <QskPopup.h>
#include <QskSeparator.h>
#include <QskTextLabel.h>

#include <QDateTime>
#include <QDate>
#include <QLocale>
#include <QTimer>
#include <QQuickWindow>

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

QSK_SUBCONTROL( HeaderElementsBox, Panel )

class HeaderElementsBox::PrivateData
{
  public:
    Position pos;
    HeaderElementsBackgroundBox* backgroundBox;
};

class MainBox::PrivateData
{
  public:
    QskLinearBox* headerBox;
    QskGraphicLabel* logo;
    HeaderElementsBox* leftElements;
    HeaderElementsBox* rightElements;
    QPointer<QskTextLabel> dateTimeLabel;
    QTimer* timer = nullptr;

    QskLinearBox* contentBox;
    QskLinearBox* sidebarBox;
    Cube* cube;
    QskGridBox* mainTileArea;
};

class MainBox::FadeAnimator : public QskAnimator
{
  public:
    FadeAnimator( MainBox* mainBox )
        : m_mainBox( mainBox )
    {
        setDuration( 300 );
        setEasingCurve( QEasingCurve::InOutQuad );
        setWindow( mainBox->window() );
    }

    void startFadeOut()
    {
        m_fadeOut = true;
        start();
    }

    void startFadeIn()
    {
        m_fadeOut = false;
        start();
    }

  protected:
    void advance( qreal value ) override
    {
        if ( m_fadeOut )
        {
            // Fade out: 1.0 -> 0.0
            if ( m_mainBox->m_data->headerBox)
                m_mainBox->m_data->headerBox->setOpacity( 1.0 - value );
            if ( m_mainBox->m_data->mainTileArea)
                m_mainBox->m_data->mainTileArea->setOpacity( 1.0 - value );
        }
        else
        {
            // Fade in: 0.0 -> 1.0
            if ( m_mainBox->m_data->headerBox)
                m_mainBox->m_data->headerBox->setOpacity( value );
            if ( m_mainBox->m_data->mainTileArea)
                m_mainBox->m_data->mainTileArea->setOpacity( value );
            if ( m_mainBox->m_data->sidebarBox)
                m_mainBox->m_data->sidebarBox->setOpacity( 1.0 );
        }
    }

    void done() override
    {
        if ( m_fadeOut )
        {
            // Fade out completed, now rebuild UI and start fade in
            m_mainBox->performRebuild();
            startFadeIn();
        }
        else
        {
            // Fade in completed, animation done
            if ( m_mainBox->m_data->headerBox)
                m_mainBox->m_data->headerBox->setOpacity( 1.0 );
            if ( m_mainBox->m_data->mainTileArea)
                m_mainBox->m_data->mainTileArea->setOpacity( 1.0 );
        }
    }

  private:
    MainBox* m_mainBox;
    bool m_fadeOut = true;
};

QSK_SUBCONTROL( HeaderElementsBackgroundBox, Panel )

// just for displaying shadow in another color and avoiding a skinlet:
HeaderElementsBackgroundBox::HeaderElementsBackgroundBox( QQuickItem* parent )
    : QskBox( parent )
{
    setSubcontrolProxy( QskBox::Panel, Panel );
}

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

MainBox::~MainBox()
{
    delete m_fadeAnimator;
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


    m_data->dateTimeLabel = new QskTextLabel( m_data->leftElements );
    m_data->dateTimeLabel->setSizePolicy( Qt::Horizontal, QskSizePolicy::Fixed );
    m_data->dateTimeLabel->setAlignment( Qt::AlignCenter );

    auto updateDateTime = [this]()
    {
        auto l = QLocale();
        auto cdt = QDateTime::currentDateTime();

        auto day = l.dayName( QDate::currentDate().dayOfWeek() );
        auto date = cdt.toString( l.dateFormat( QLocale::ShortFormat ) );
        auto t = cdt.toString( l.timeFormat( QLocale::ShortFormat ) );

        if( m_data->dateTimeLabel )
            m_data->dateTimeLabel->setText( QString( "%1\n%2\n%3").arg( day ).arg( date ).arg( t ) );
    };

    if( !m_data->timer )
    {
        m_data->timer = new QTimer( this );
        connect( m_data->timer, &QTimer::timeout, this, updateDateTime );
    }

    m_data->timer->start( 1000 * 60 );

    updateDateTime();

    new QskSeparator( Qt::Vertical, m_data->leftElements );

    auto* bluetoothSwitch = new Switch( "bluetooth", m_data->leftElements );

    new QskSeparator( Qt::Vertical, m_data->leftElements );

    auto* userButton = new Button( "user", m_data->leftElements );
    userButton->setText( tr("Hans") );


    auto* wifiSwitch = new Switch( "wifi", m_data->rightElements );

    new QskSeparator( Qt::Vertical, m_data->rightElements );

    auto* mapsSwitch = new Switch( "map-location", m_data->rightElements );

    new QskSeparator( Qt::Vertical, m_data->rightElements );

    auto* settingsButton = new Button( "bars", m_data->rightElements );
    settingsButton->setText( tr("settings") );
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

    const QVector< QPair< QString, std::function< void() > > > sidebarEntries = {
        { "house", [this]()
            {
                m_data->cube->switchToPosition( Cube::FrontPos );
         } },
        { "cube", [this]()
            {
                m_data->cube->switchPosition( Qsk::LeftToRight );
         } },
        { "moon", [this]()
            {
                QskSkin::ColorScheme colorScheme;
                QString iconSource;

                auto* skin = qskSkinManager->skin();

                if( skin->colorScheme() == QskSkin::LightScheme )
                {
                    colorScheme = QskSkin::DarkScheme;
                    iconSource = "sun";
                }
                else
                {
                    colorScheme = QskSkin::LightScheme;
                    iconSource = "moon";
                }

                skin->setColorScheme( colorScheme );

                auto* button = qobject_cast< Button* >( sender() );
                button->setIconSource( iconSource );
         } },
        { "language", [this]()
            {
                auto* button = qobject_cast< Button* >( sender() );

                const QVector< QskLabelData > options = {
                    { "Deutsch", { "country-de" } },
                    { "English", { "country-us" } },
                    { "中国人", { "country-cn" } } };

                const QVector< QLocale > locales = { QLocale( "de_DE" ), QLocale( "en_US" ), QLocale( "zh_CN" ) };
                const int index = locales.indexOf( QLocale().language() );

                auto* popup = new SidebarButtonPopup( button, options, index );

                connect( popup, &SidebarButtonPopup::selectedIndexChanged, this, [this, popup, locales]( int index )
                {
                    QLocale::setDefault( locales.at( index ) );

                    const auto localeName = locales.at( index ).name();

                    QTimer::singleShot( 100, popup, [this, popup, localeName]()
                    {
                        popup->close();
                        Q_EMIT languageChanged( localeName );

                    } );
                } );

                popup->toggle();
         } },
        { "tractor", []()
            {
         } },
        { "road", []()
            {
         } }
    };

    for( const auto& entry : sidebarEntries )
    {
        auto* b = new Button( entry.first, m_data->sidebarBox );
        b->setType( Button::Type::Sidebar );
        connect( b, &QskPushButton::clicked, this, entry.second );
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
#if 1 // change to use brand logos
        Q_UNUSED( url )
#else
        m_data->logo->setSource( url );
#endif
    } );

    auto* powerLiftTile = new FrontPowerLiftTile( m_data->mainTileArea );
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

void MainBox::rebuildUI()
{
    // Start fade out animation instead of immediate rebuild
    startFadeAnimation();
}

void MainBox::startFadeAnimation()
{
    if ( !m_fadeAnimator )
    {
        m_fadeAnimator = new FadeAnimator( this );
    }

    if ( !m_fadeAnimator->isRunning() )
    {
        m_fadeAnimator->startFadeOut();
    }
}

void MainBox::performRebuild()
{
    m_data->timer->stop();

    // This is called when fade out is complete
    // Clear existing content
    if (m_data->headerBox) {
        m_data->headerBox->deleteLater();
        m_data->headerBox = nullptr;
    }
    if (m_data->contentBox) {
        m_data->contentBox->deleteLater();
        m_data->contentBox = nullptr;
    }
    if (m_data->cube) {
        m_data->cube->deleteLater();
        m_data->cube = nullptr;
    }
    if (m_data->mainTileArea) {
        m_data->mainTileArea->deleteLater();
        m_data->mainTileArea = nullptr;
    }

    // Rebuild the UI
    setupHeaderBox();
    setupContentBox();

    m_data->headerBox->setOpacity( 0.0 );
    m_data->sidebarBox->setOpacity( 0.0 );
    m_data->mainTileArea->setOpacity( 0.0 );
}

#include "moc_MainBox.cpp"

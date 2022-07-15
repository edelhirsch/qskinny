#include "MainItem.h"

#include "Cube.h"
#include "DashboardPage.h"
#include "MenuBar.h"
#include "RoomsPage.h"

#include <QskGesture.h>
#include <QskEvent.h>
#include <QskLinearBox.h>

#include <QQuickFramebufferObject>
#include <QGuiApplication>
#include <QQuickWindow>
#include <QTimer>

MainItem::MainItem( QQuickItem* parent )
    : QskControl( parent )
    , m_dashboardLayout( new QskLinearBox( Qt::Horizontal, this ) )
    , m_cube( new Cube( this ) )
{
    setAutoLayoutChildren( true );
    setAcceptedMouseButtons( Qt::LeftButton );
    setFiltersChildMouseEvents( true );

    m_panRecognizer.setOrientations( Qt::Horizontal );
    m_panRecognizer.setMinDistance( 50 );
    m_panRecognizer.setWatchedItem( this );

    m_dashboardLayout->setSpacing( 0 );

    m_roomsLayout = new QskLinearBox( Qt::Horizontal, m_offscreenWindow.contentItem() );
    m_roomsLayout->setSpacing( 0 );
    new MenuBar( m_roomsLayout );
    new RoomsPage( m_roomsLayout );

    m_frontLayout = m_dashboardLayout;
    m_otherLayout = m_roomsLayout;

    connect( window(), &QWindow::widthChanged, this, [ this ]()
    {
        m_offscreenWindow.setGeometry( window()->geometry() );
        m_offscreenWindow.create();
        m_roomsLayout->setSize( m_offscreenWindow.size() );
    } );

    m_cube->setVisible( false );

    (void) new MenuBar( m_dashboardLayout );
    (void) new DashboardPage( m_dashboardLayout );

    connect( m_cube, &Cube::animationFinished, this, [ this ]()
    {
        if( m_frontLayout == m_dashboardLayout )
        {
            m_dashboardLayout->setParentItem( m_offscreenWindow.contentItem() );
            m_roomsLayout->setParentItem( window()->contentItem() );

            m_frontLayout = m_roomsLayout;
            m_otherLayout = m_dashboardLayout;
        }
        else
        {
            m_dashboardLayout->setParentItem( window()->contentItem() );
            m_roomsLayout->setParentItem( m_offscreenWindow.contentItem() );

            m_frontLayout = m_dashboardLayout;
            m_otherLayout = m_roomsLayout;
        }

        m_cube->setVisible( false );
        m_frontLayout->setVisible( true );
    } );
}

void MainItem::gestureEvent( QskGestureEvent* event )
{
    if( event->gesture()->state() == QskGesture::Finished
            && event->gesture()->type() == QskGesture::Pan )
    {
        auto* panGesture = static_cast< const QskPanGesture* >( event->gesture().get() );
        const Cube::Position pos = ( panGesture->origin().x() < panGesture->position().x() )
                ? Cube::Left : Cube::Right;

        m_cube->setCurrentPosition( pos );

        m_otherLayout->setVisible( true );

        const auto frontImage = window()->grabWindow();
        m_cube->setImage( Cube::Front, frontImage );

        const auto otherImage = m_offscreenWindow.grabWindow();
        m_cube->setImage( pos, otherImage );

        m_frontLayout->setVisible( false );

        m_cube->setVisible( true );
        m_cube->startAnimation();

    }
}

bool MainItem::gestureFilter( QQuickItem* item, QEvent* event )
{
    auto& recognizer = m_panRecognizer;

    if( event->type() == QEvent::MouseButtonPress )
    {
        const auto mouseEvent = static_cast< QMouseEvent* >( event );

        if( ( item != this ) || ( recognizer.timeout() < 0 ) )
        {
            if( recognizer.hasProcessedBefore( mouseEvent ) )
            {
                return false;
            }
        }

        recognizer.setTimeout( ( item == this ) ? -1 : 100 );
    }

    return recognizer.processEvent( item, event, false );
}

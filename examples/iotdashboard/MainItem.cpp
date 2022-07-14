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
    , m_mainLayout( new QskLinearBox( Qt::Horizontal, this ) )
    , m_cube( new Cube( this ) )
{
    setAutoLayoutChildren( true );
    setAcceptedMouseButtons( Qt::LeftButton );
    setFiltersChildMouseEvents( true );

    m_panRecognizer.setOrientations( Qt::Horizontal );
    m_panRecognizer.setMinDistance( 50 );
    m_panRecognizer.setWatchedItem( this );

    m_mainLayout->setSpacing( 0 );

    m_otherLayout = new QskLinearBox( Qt::Horizontal, m_offscreenWindow.contentItem() );
    m_otherLayout->setSpacing( 0 );
    new MenuBar( m_otherLayout );
    new RoomsPage( m_otherLayout );

    connect( window(), &QWindow::widthChanged, this, [ this ]()
    {
        m_offscreenWindow.setGeometry( window()->geometry() );
        m_offscreenWindow.create();
        m_otherLayout->setSize( m_offscreenWindow.size() );
    } );

//    QTimer::singleShot( 1000, this, [this]() {
//        auto i = m_offscreenWindow.grabWindow();
//        qDebug()  << i << m_offscreenWindow.size();
//        i.save("/tmp/other.png");
//    });

    m_cube->setVisible( false );

    (void) new MenuBar( m_mainLayout );
    (void) new DashboardPage( m_mainLayout );

    connect( m_cube, &Cube::animationFinished, this, [ this ]()
    {
        m_otherLayout->setParentItem( window()->contentItem() );
    } );
}

void MainItem::gestureEvent( QskGestureEvent* event )
{
    if( event->gesture()->state() == QskGesture::Finished )
    {
        const auto frontImage = window()->grabWindow();
        m_cube->setImage( Cube::Front, frontImage );

        const auto otherImage = m_offscreenWindow.grabWindow();
        m_cube->setImage( Cube::Right, otherImage );

        m_mainLayout->setVisible( false );
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

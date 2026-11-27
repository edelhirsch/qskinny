/******************************************************************************
 * QSkinny - Copyright (C) The authors
 *           SPDX-License-Identifier: BSD-3-Clause
 *****************************************************************************/

#include <SkinnyNamespace.h>

#include <QskControl.h>
#include <QskPopup.h>
#include <QskWindow.h>

#include <QTimer>
#include <QtTest>

namespace
{
    class Popup : public QskPopup
    {
      public:
        using QskPopup::execPopup;

        void mousePressEvent( QMouseEvent* event ) override
        {
            QskPopup::mousePressEvent( event );

            pressed = true;
            close();
        }

        bool pressed = false;
    };

    class Trigger : public QskControl
    {
      public:
        Trigger( Popup* popup )
            : m_popup( popup )
        {
            setAcceptedMouseButtons( Qt::LeftButton );
        }

        void mousePressEvent( QMouseEvent* ) override
        {
        }

        void mouseReleaseEvent( QMouseEvent* ) override
        {
            // closing the popup, when the next touch got lost
            QTimer::singleShot( 200, m_popup, &QskPopup::close );
            m_popup->execPopup();
        }

      private:
        Popup* m_popup;
    };
}

class tst_QskPopup : public QObject
{
    Q_OBJECT

  private Q_SLOTS:
    void initTestCase();
    void touchAfterExec();
};

void tst_QskPopup::initTestCase()
{
    Skinny::init();
}

/*
    A popup is opened with exec() from the release of a touch.
    The very next touch must not get lost.
 */
void tst_QskPopup::touchAfterExec()
{
    auto device = QTest::createTouchDevice();

    QskWindow window;
    window.resize( 200, 200 );

    auto popup = new Popup;
    window.addItem( new Trigger( popup ) );

    // the popup needs to be stacked above the trigger
    popup->setParentItem( window.contentItem() );
    popup->setGeometry( 0, 0, 200, 200 );

    window.show();
    QVERIFY( QTest::qWaitForWindowExposed( &window ) );

    // queued, so that the touch is sent from the event loop of exec()
    connect( popup, &QskPopup::opened, popup, [&]() {
        QTest::touchEvent( &window, device ).press( 0, QPoint( 100, 100 ) ).commit();
        QTest::touchEvent( &window, device ).release( 0, QPoint( 100, 100 ) ).commit();
    }, Qt::QueuedConnection );

    auto touch = QTest::touchEvent( &window, device );
    touch.press( 0, QPoint( 100, 100 ) ).commit();
    touch.release( 0, QPoint( 100, 100 ) ).commit();

    QVERIFY( popup->pressed );
}

QTEST_MAIN( tst_QskPopup )

#include "tst_qskpopup.moc"

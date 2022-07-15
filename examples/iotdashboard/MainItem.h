#pragma once

#include <QskControl.h>
#include <QskPanGestureRecognizer.h>

#include <QQuickWindow>

class Cube;

class QskBox;
class QskLinearBox;

class MainItem : public QskControl
{
    Q_OBJECT

    public:
        MainItem( QQuickItem* parent = nullptr );

    protected:
        bool gestureFilter( QQuickItem*, QEvent* ) override final;
        void gestureEvent( QskGestureEvent* ) override final;

    private:
        QskLinearBox* m_dashboardLayout;
        QskLinearBox* m_roomsLayout;
        QskLinearBox* m_frontLayout; // points to one of the above
        QskLinearBox* m_otherLayout; // points to one of the above
        QQuickWindow m_offscreenWindow;
        Cube* m_cube;
        QskPanGestureRecognizer m_panRecognizer;
};

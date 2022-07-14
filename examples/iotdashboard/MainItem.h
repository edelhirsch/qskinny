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
        QskLinearBox* m_mainLayout;
        QskLinearBox* m_otherLayout;
        QQuickWindow m_offscreenWindow;
        Cube* m_cube;
        QskPanGestureRecognizer m_panRecognizer;
};

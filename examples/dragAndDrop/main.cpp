/******************************************************************************
 * QSkinny - Copyright (C) 2016 Uwe Rathmann
 * This file may be used under the terms of the 3-clause BSD License
 *****************************************************************************/

#include <SkinnyFont.h>
#include <SkinnyShortcut.h>

#include <QskLinearBox.h>
#include <QskTextLabel.h>
#include <QskWindow.h>

#include <QGuiApplication>
#include <QMimeData>
#include <QDrag>

class DraggableItem : public QskTextLabel
{
    Q_OBJECT

public:
    DraggableItem( const QString& text, QQuickItem* parent = nullptr ) : QskTextLabel( text, parent )
    {
        setPanel( true );
        setAcceptedMouseButtons(Qt::AllButtons);
        setAcceptTouchEvents(true);
        setFlag( ItemAcceptsDrops, true );
        setBackground( Qt::cyan );
    }

protected:
    void mousePressEvent(QMouseEvent* event) override
    {
        if (event->button() == Qt::LeftButton) {
            auto* drag = new QDrag( this );
            auto* mimeData = new QMimeData;
            mimeData->setData( "text/html", "bla" );
            drag->setMimeData( mimeData );
            drag->exec();
        }
    }

    void dragEnterEvent(QDragEnterEvent *event) override
    {
        qDebug() << Q_FUNC_INFO << event;
        QskTextLabel::dragEnterEvent( event );
    }

    void dragLeaveEvent(QDragLeaveEvent *event) override
    {
        qDebug() << Q_FUNC_INFO;
        QskTextLabel::dragLeaveEvent( event );
    }

    void dragMoveEvent(QDragMoveEvent *event) override
    {
        qDebug() << Q_FUNC_INFO;
        QskTextLabel::dragMoveEvent( event );
    }

    void dropEvent(QDropEvent *event) override
    {
        qDebug() << Q_FUNC_INFO;
        QskTextLabel::dropEvent( event );
    }
};

int main( int argc, char* argv[] )
{
    QGuiApplication app( argc, argv );

    SkinnyFont::init( &app );
    SkinnyShortcut::enable( SkinnyShortcut::AllShortcuts );

    auto horizontalBox = new QskLinearBox( Qt::Horizontal );
    horizontalBox->setDefaultAlignment( Qt::AlignLeft );
    horizontalBox->setMargins( 10 );
    horizontalBox->setSpacing( 10 );

    auto* label1 = new DraggableItem( "draggable item 1" );
    label1->setMargins( 10 );
    label1->setBackgroundColor( Qt::magenta );
    horizontalBox->addItem( label1 );

    horizontalBox->addSpacer( 200 );

    auto* label2 = new DraggableItem( "draggable item 2" );
    label2->setMargins( 10 );
    label2->setBackgroundColor( Qt::magenta );
    horizontalBox->addItem( label2 );

    QskWindow window;
    window.addItem( horizontalBox );
    window.show();

    return app.exec();
}

#include "main.moc"

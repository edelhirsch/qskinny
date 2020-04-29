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
            QImage image( "/home/peter/dev/qskinny/examples/dragAndDrop/item.png" );
            QPixmap pixmap = QPixmap::fromImage( image );
            drag->setPixmap( pixmap );
            auto* mimeData = new QMimeData;
            mimeData->setData( "text/html", "bla" );
            drag->setMimeData( mimeData );
            drag->exec( Qt::MoveAction );
        }
    }
};

class DropItem : public QskBox
{
    Q_OBJECT
public:
    DropItem(QQuickItem* parent = nullptr) : QskBox(parent)
    {
        setPanel( true );
        setFixedSize( 500, 100 );
        setBackground( Qt::green );

        setFlag( ItemAcceptsDrops, true );
    }

protected:
    void dragEnterEvent(QDragEnterEvent *event) override
    {
        auto* sourceItem = qobject_cast< QQuickItem* >( event->source() );
        auto* layout = qobject_cast< QskLinearBox* >( parentItem() );
        sourceItem->setParentItem( this );
        layout->insertSpacer( 0, sourceItem->width() ); // just so the item doesn't jump
        event->setAccepted( true );
    }

    void dragMoveEvent(QDragMoveEvent *event) override
    {
        QskBox::dragMoveEvent( event );
    }

    void dragLeaveEvent(QDragLeaveEvent *event) override
    {
        QskBox::dragLeaveEvent( event );
    }

    void dropEvent(QDropEvent *event) override
    {
        QskBox::dropEvent( event );
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

    auto* label = new DraggableItem( "draggable item" );
    label->setMargins( 10 );
    label->setBackgroundColor( Qt::magenta );
    horizontalBox->addItem( label );

    horizontalBox->addSpacer( 200 );

    auto* dropItem = new DropItem;
    horizontalBox->addItem( dropItem );

    QskWindow window;
    window.addItem( horizontalBox );
    window.show();

    return app.exec();
}

#include "main.moc"

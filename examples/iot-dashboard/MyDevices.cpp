#include "MyDevices.h"
#include "Skin.h"
#include "RoundedIcon.h"

#include <QskGraphic.h>
#include <QskGraphicLabel.h>
#include <QskGridBox.h>
#include <QskTextLabel.h>

#include <QImage>

namespace
{
    class Device : public QskLinearBox
    {
        public:
            Device( const QString& name, bool isBright, QQuickItem* parent )
                : QskLinearBox( Qt::Vertical, parent )
                , m_name( name )
            {
                setDefaultAlignment( Qt::AlignCenter );
                setAutoAddChildren( false );

                m_icon = new RoundedIcon( QString(), isBright, this );
                m_icon->setOpacity( 0.15 );
                m_icon->setFixedWidth( 60 );
                addItem( m_icon );

                auto* textLabel = new QskTextLabel( name, this );
                textLabel->setFontRole( QskSkin::TinyFont );
                textLabel->setAlignment( Qt::AlignHCenter );
                addItem( textLabel );

                auto fileName = name.toLower();
                fileName.replace( ' ', '-' );
                fileName = ":/images/" + fileName + ".png";
                QImage image( fileName );
                auto graphic = QskGraphic::fromImage( image );
                m_graphicLabel = new QskGraphicLabel( graphic, this );
            }

        protected:
            void updateLayout() override
            {
                QskLinearBox::updateLayout();

                m_graphicLabel->setSize( {36, 36} );
                m_graphicLabel->setPosition( { m_icon->position().x() + ( m_icon->width() - m_graphicLabel->width() ) / 2,
                                               ( m_icon->position().y() + m_icon->height() - m_graphicLabel->height() ) / 2 } );
            }

        private:
            QString m_name;
            RoundedIcon* m_icon;
            QskGraphicLabel* m_graphicLabel;
    };
}

MyDevices::MyDevices( QQuickItem* parent )
    : Box( "My devices", parent )
{
    auto* content = new QskGridBox( this );

    auto* lamps = new Device( "Lamps", true, content );
    content->addItem( lamps, 0, 0 );

    auto* musicSystem = new Device( "Music System", false, content );
    content->addItem( musicSystem, 0, 1 );

    auto* ac = new Device( "AC", false, content );
    content->addItem( ac, 1, 0 );

    auto* router = new Device( "Router", true, content );
    content->addItem( router, 1, 1 );
}

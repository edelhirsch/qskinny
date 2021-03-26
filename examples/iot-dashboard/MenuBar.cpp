#include "MenuBar.h"

#include <QskGraphic.h>
#include <QskGraphicLabel.h>
#include <QskGraphicIO.h>
#include <QskSkin.h>
#include <QskTextLabel.h>

#include <QtGui/QImage>

QSK_SUBCONTROL( MenuItem, Panel )

MenuItem::MenuItem( const QString& name, QQuickItem* parent ) : QskLinearBox( Qt::Horizontal, parent ),
    m_name( name )
{
    setAutoLayoutChildren( true );
    setAutoAddChildren( true );
    setSizePolicy( QskSizePolicy::Fixed, QskSizePolicy::Fixed );
    setSpacing( 6 );

    setAcceptHoverEvents( true );
    setPanel( true );

    QString fileName = ":/images/" + name.toLower() + ".png"; // width: 14
    QImage image( fileName );
    auto graphic = QskGraphic::fromImage( image );
    auto* graphicLabel = new QskGraphicLabel( graphic, this );
    graphicLabel->setSizePolicy( QskSizePolicy::Fixed, QskSizePolicy::Fixed );
    graphicLabel->setFixedWidth( 14 );
    graphicLabel->setAlignment( Qt::AlignCenter );

    auto* textLabel = new QskTextLabel( name, this );
    textLabel->setTextColor( Qt::white ); // ### style
    textLabel->setFontRole( QskSkin::SmallFont ); // ### style
}

QskAspect::Subcontrol MenuItem::effectiveSubcontrol( QskAspect::Subcontrol subControl ) const
{
    if( subControl == QskBox::Panel )
    {
        return MenuItem::Panel;
    }

    return subControl;
}

void MenuItem::setActive( bool active )
{
    QColor color;

    if( active )
    {
        color = Qt::white;
        color.setAlphaF( 0.14 );
    }
    else
    {
        color = Qt::transparent;
    }

    setBackgroundColor( color );
}


QSK_SUBCONTROL( MenuBar, Panel )

MenuBar::MenuBar( QQuickItem* parent ) : QskLinearBox( Qt::Vertical, parent )
{
    setPanel( true );
    setSizePolicy( QskSizePolicy::Minimum, QskSizePolicy::Preferred );
    setSpacing( 8 );

    auto* mainIcon = ":/images/main-icon.png";
    QImage image( mainIcon );
    auto graphic = QskGraphic::fromImage( image );
    auto* graphicLabel = new QskGraphicLabel( graphic, this );
    graphicLabel->setAlignment( Qt::AlignTop );
    graphicLabel->setMargins( { 50, 0, 50, 54 } );
    graphicLabel->setSizePolicy( QskSizePolicy::Fixed, QskSizePolicy::Fixed );

    m_entryStrings = { "Dashboard", "Rooms", "Devices", "Statistics", "Storage", "Members" };

    for( const auto& entryString : qAsConst( m_entryStrings ) )
    {
        auto* entry = new MenuItem( entryString, this );
        m_entries.append( entry );
    }

    m_entries.at( m_activeEntry )->setActive( true );

    addSpacer( 0, 1 ); // fill the space at the bottom

    new MenuItem( "Logout", this );
}

QskAspect::Subcontrol MenuBar::effectiveSubcontrol( QskAspect::Subcontrol subControl ) const
{
    if( subControl == QskBox::Panel )
    {
        return MenuBar::Panel;
    }

    return subControl;
}

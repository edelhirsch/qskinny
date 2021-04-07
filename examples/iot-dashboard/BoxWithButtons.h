#ifndef BOXWITHBUTTONS_H
#define BOXWITHBUTTONS_H

#include "Box.h"
#include "RoundedIcon.h"

#include <QskBoxShapeMetrics.h>
#include <QskTextLabel.h>

class ButtonValueLabel : public QskTextLabel
{
        Q_OBJECT

    public:
        QSK_SUBCONTROLS( Text )

        ButtonValueLabel( const QString& text, QQuickItem* parent ) : QskTextLabel( text, parent )
        {
        }

        QskAspect::Subcontrol effectiveSubcontrol(
            QskAspect::Subcontrol subControl ) const override final
        {
            if( subControl == QskTextLabel::Text )
            {
                return Text;
            }

            return subControl;
        }
};

class BoxWithButtons : public Box
{
    public:
        QSK_SUBCONTROLS( Panel )

        BoxWithButtons( const QString& title, const QString& value, bool isBright, QQuickItem* parent = nullptr );

        QskAspect::Subcontrol effectiveSubcontrol(
            QskAspect::Subcontrol subControl ) const override
        {
            if( subControl == QskBox::Panel )
            {
                return Panel;
            }

            return subControl;
        }
};

class IndoorTemperature : public BoxWithButtons
{
    public:
        QSK_SUBCONTROLS( Panel )

        IndoorTemperature( QQuickItem* parent )
            : BoxWithButtons( "Indoor Temperature", "+24", true, parent )
        {
        }
};

class Humidity: public BoxWithButtons
{
    public:
        QSK_SUBCONTROLS( Panel )

        Humidity( QQuickItem* parent )
            : BoxWithButtons( "Humidity", "30%", false, parent )
        {
        }
};

#endif // BOXWITHBUTTONS_H
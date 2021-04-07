#ifndef UPANDDOWNBUTTON_H
#define UPANDDOWNBUTTON_H

#include <QskLinearBox.h>
#include <QskPushButton.h>

class RoundButton : QskPushButton
{
        Q_OBJECT

    public:
        QSK_SUBCONTROLS( Panel )
        QSK_STATES( Top )

        RoundButton( QskAspect::Placement placement, QQuickItem* parent );

        QskAspect::Subcontrol effectiveSubcontrol(
            QskAspect::Subcontrol subControl ) const override final;
};

class UpAndDownButton : public QskLinearBox
{
        Q_OBJECT

    public:
        UpAndDownButton( QQuickItem* parent );
};

#endif // UPANDDOWNBUTTON_H

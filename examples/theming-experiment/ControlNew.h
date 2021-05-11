/******************************************************************************
 * QSkinny - Copyright (C) 2021 Uwe Rathmann
 * This file may be used under the terms of the QSkinny License, Version 1.0
 *****************************************************************************/

#ifndef CONTROL_NEW_H
#define CONTROL_NEW_H

#include <QskControl.h>
#include <QskNamespace.h>

class ControlNew : public QskControl
{
        Q_OBJECT

        using Inherited = QskControl;

    public:
        QSK_SUBCONTROLS( Subcontrol1, Subcontrol2, Subcontrol3, Subcontrol4,
                         Text1, Text2, Text3, Text4 )

        ControlNew( QQuickItem* parent = nullptr );
        ~ControlNew() override;

    protected:
        QSizeF contentsSizeHint( Qt::SizeHint, const QSizeF& ) const override;

    private:
        class PrivateData;
        std::unique_ptr< PrivateData > m_data;
};

#endif // CONTROL_NEW_H

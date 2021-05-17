/******************************************************************************
 * QSkinny - Copyright (C) 2021 Uwe Rathmann
 * This file may be used under the terms of the QSkinny License, Version 1.0
 *****************************************************************************/

#ifndef CONTROL_OLD_H
#define CONTROL_OLD_H

#include <QskControl.h>
#include <QskNamespace.h>

class ControlOld : public QskControl
{
        Q_OBJECT

        using Inherited = QskControl;

    public:
        QSK_SUBCONTROLS( Subcontrol1, Subcontrol2, Subcontrol3, Subcontrol4,
                         Text1, Text2, Text3, Text4 )

        ControlOld( QQuickItem* parent = nullptr );
        ~ControlOld() override;

    protected:
        QSizeF contentsSizeHint( Qt::SizeHint, const QSizeF& ) const override;

    private:
        class PrivateData;
        std::unique_ptr< PrivateData > m_data;
};

#endif // CONTROL_OLD_H

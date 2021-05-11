/******************************************************************************
 * QSkinny - Copyright (C) 2021 Uwe Rathmann
 * This file may be used under the terms of the QSkinny License, Version 1.0
 *****************************************************************************/

#include "ControlOld.h"

class ControlOld::PrivateData
{
    public:
        PrivateData()
        {
        }
};

QSK_SUBCONTROL( ControlOld, Subcontrol1 )
QSK_SUBCONTROL( ControlOld, Subcontrol2 )
QSK_SUBCONTROL( ControlOld, Subcontrol3 )
QSK_SUBCONTROL( ControlOld, Subcontrol4 )
QSK_SUBCONTROL( ControlOld, Text1 )
QSK_SUBCONTROL( ControlOld, Text2 )
QSK_SUBCONTROL( ControlOld, Text3 )
QSK_SUBCONTROL( ControlOld, Text4 )

ControlOld::ControlOld( QQuickItem* parent )
    : Inherited( parent )
    , m_data( new PrivateData() )
{
}

ControlOld::~ControlOld()
{
}

QSizeF ControlOld::contentsSizeHint( Qt::SizeHint /*which*/, const QSizeF& ) const
{
    return {300, 200};
}

#include "moc_ControlOld.cpp"

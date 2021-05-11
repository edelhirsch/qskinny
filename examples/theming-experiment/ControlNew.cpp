/******************************************************************************
 * QSkinny - Copyright (C) 2021 Uwe Rathmann
 * This file may be used under the terms of the QSkinny License, Version 1.0
 *****************************************************************************/

#include "ControlNew.h"

class ControlNew::PrivateData
{
    public:
        PrivateData()
        {
        }
};

QSK_SUBCONTROL( ControlNew, Subcontrol1 )
QSK_SUBCONTROL( ControlNew, Subcontrol2 )
QSK_SUBCONTROL( ControlNew, Subcontrol3 )
QSK_SUBCONTROL( ControlNew, Subcontrol4 )
QSK_SUBCONTROL( ControlNew, Text1 )
QSK_SUBCONTROL( ControlNew, Text2 )
QSK_SUBCONTROL( ControlNew, Text3 )
QSK_SUBCONTROL( ControlNew, Text4 )

ControlNew::ControlNew( QQuickItem* parent )
    : Inherited( parent )
    , m_data( new PrivateData() )
{
}

ControlNew::~ControlNew()
{
}

QSizeF ControlNew::contentsSizeHint( Qt::SizeHint /*which*/, const QSizeF& ) const
{
    return {300, 200};
}

#include "moc_ControlNew.cpp"

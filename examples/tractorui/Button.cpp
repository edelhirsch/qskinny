/******************************************************************************
 * Copyright (C) The authors
 *           SPDX-License-Identifier: BSD-3-Clause
 *****************************************************************************/

#include "Button.h"

#include <QskGraphic.h>
#include <QskGraphicIO.h>

QSK_SUBCONTROL( Button, Panel )
QSK_SUBCONTROL( Button, InnerPanel1 )
QSK_SUBCONTROL( Button, InnerPanel2 )
QSK_SUBCONTROL( Button, InnerPanel3 )
QSK_SUBCONTROL( Button, Icon )
QSK_SUBCONTROL( Button, Text )

Button::Button( const QString& iconSource, QQuickItem* parent )
    : QskPushButton( parent )
    , m_type( Type::Normal )
{
    if( !iconSource.isEmpty() )
    {
        setIconSource( iconSource );
    }

    setSizePolicy( Qt::Horizontal, QskSizePolicy::Fixed );

    setSubcontrolProxy( QskPushButton::Panel, Panel );
    setSubcontrolProxy( QskPushButton::Icon, Icon );
    setSubcontrolProxy( QskPushButton::Text, Text );
}

Button::Type Button::type() const
{
    return m_type;
}

void Button::setType( Type type )
{
    m_type = type;
}

QskAspect::Variation Button::effectiveVariation() const
{
    return static_cast< QskAspect::Variation >( m_type );
}

#include "moc_Button.cpp"

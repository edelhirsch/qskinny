/******************************************************************************
 * Copyright (C) The authors
 *           SPDX-License-Identifier: BSD-3-Clause
 *****************************************************************************/

#include "Switch.h"

#include <QskSwitchButton.h>

QSK_SUBCONTROL( SwitchLabel, Panel )
QSK_SUBCONTROL( SwitchLabel, Graphic )

SwitchLabel::SwitchLabel( const QString& iconSource, QQuickItem* parent )
    : QskGraphicLabel( iconSource, parent )
{
    setSubcontrolProxy( QskGraphicLabel::Graphic, Graphic );
}

class Switch::PrivateData
{
  public:
    SwitchLabel* icon;
    QskSwitchButton* switchButton;
};

Switch::Switch( const QString& iconSource, QQuickItem* parent )
    : QskLinearBox( Qt::Vertical, parent )
    , m_data( new PrivateData )
{
    m_data->icon = new SwitchLabel( iconSource, this );
    m_data->switchButton = new QskSwitchButton( this );

    setSizePolicy( QskSizePolicy::Fixed, QskSizePolicy::Fixed );
    setSpacing( 10 ); // ### style
}

Switch::~Switch()
{
}

/******************************************************************************
 * Copyright (C) 2021 Edelhirsch Software GmbH
 * This file may be used under the terms of the 3-clause BSD License
 *****************************************************************************/

#include "MainWindow.h"

#include "ControlNew.h"
#include "ControlOld.h"

#include <QskLinearBox.h>

MainWindow::MainWindow()
    : QskWindow()
{
    setPreferredSize( { 1024, 600 } );
    setTitle( "Theming experiment" );

    auto* box = new QskLinearBox( Qt::Horizontal, contentItem() );
    box->setDefaultAlignment( Qt::AlignCenter | Qt::AlignHCenter );
    new ControlOld( box );
    new ControlNew( box );
}

#include "moc_MainWindow.cpp"

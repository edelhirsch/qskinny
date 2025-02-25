/******************************************************************************
 * Copyright (C) 2021 Edelhirsch Software GmbH
 *           SPDX-License-Identifier: BSD-3-Clause
 *****************************************************************************/

#include "MainBox.h"
#include "MainWindow.h"

#include <QskLinearBox.h>

MainWindow::MainWindow()
{
    setPreferredSize( QSize( 1280, 800 ) );
    setTitle( "Tractor UI" );

    (void) new MainBox( contentItem() );
}

#include "moc_MainWindow.cpp"

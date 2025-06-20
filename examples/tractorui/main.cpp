/******************************************************************************
 * Copyright (C) 2021 Edelhirsch Software GmbH
 *           SPDX-License-Identifier: BSD-3-Clause
 *****************************************************************************/

#include "MainWindow.h"
#include "GraphicProvider.h"
#include "Skin.h"

#ifdef USE_SHORTCUTS
#include <SkinnyShortcut.h>
#endif

#include <QskSkinManager.h>
#include <QskSetup.h>
#include <QskShortcutMap.h>
#include <QskWindow.h>
#include <QskObjectCounter.h>

#include <QGuiApplication>
#include <QTimer>
#include <QTranslator>
#include <QLibraryInfo>
#include <QDir>

int main( int argc, char* argv[] )
{
#ifdef ITEM_STATISTICS
    QskObjectCounter counter( true );
#endif

    // Use system locale by default
    QLocale::setDefault( QLocale( "en_US" ) );

    QGuiApplication app( argc, argv );

    qskSkinManager->setSkin( new Skin() );

    Qsk::addGraphicProvider( {}, new GraphicProvider() );

#ifdef USE_SHORTCUTS
    SkinnyShortcut::enable( SkinnyShortcut::ChangeColorScheme
        | SkinnyShortcut::DebugBackground | SkinnyShortcut::DebugStatistics
        | SkinnyShortcut::Quit );
#endif

    MainWindow window;
    window.show();

    return app.exec();
}

/******************************************************************************
 * Copyright (C) 2021 Edelhirsch Software GmbH
 * This file may be used under the terms of the 3-clause BSD License
 *****************************************************************************/

#include "MainWindow.h"

#include "Skin.h"

#include <SkinnyFont.h>
#include <SkinnyShortcut.h>

#include <QskSetup.h>
#include <QskShortcutMap.h>
#include <QskSkinFactory.h>
#include <QskSkinManager.h>
#include <QskWindow.h>

#include <QGuiApplication>


namespace
{
    class SkinFactory : public QskSkinFactory
    {
            Q_OBJECT

        public:
            SkinFactory( QObject* parent = nullptr ) : QskSkinFactory( parent )
            {
            }

            QStringList skinNames() const override
            {
                return { "Skin" };
            }

            QskSkin* createSkin( const QString& skinName ) override
            {
                if( skinName == "Skin" )
                {
                    return new Skin;
                }

                return nullptr;
            }
    };
}

int main( int argc, char* argv[] )
{
    QGuiApplication app( argc, argv );

    SkinnyFont::init( &app );

    SkinFactory skinFactory;

    qskSkinManager->setPluginPaths( QStringList() ); // no plugins

    qskSkinManager->unregisterFactory( "materialfactory" );
    qskSkinManager->unregisterFactory( "squiekfactory" );
    qskSkinManager->registerFactory(
        QStringLiteral( "SampleSkinFactory" ), &skinFactory );

    qskSetup->setSkin( "Skin" );

    // With CTRL-B you can rotate a couple of visual debug modes
    SkinnyShortcut::enable( SkinnyShortcut::RotateSkin | SkinnyShortcut::DebugBackground |
                            SkinnyShortcut::DebugStatistics | SkinnyShortcut::Quit );

    MainWindow window;
    window.show();

    return app.exec();
}

#include "main.moc"

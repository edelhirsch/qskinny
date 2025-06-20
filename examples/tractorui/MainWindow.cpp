/******************************************************************************
 * Copyright (C) 2021 Edelhirsch Software GmbH
 *           SPDX-License-Identifier: BSD-3-Clause
 *****************************************************************************/

#include "MainBox.h"
#include "MainWindow.h"

#include <QskLinearBox.h>
#include <QTranslator>
#include <QCoreApplication>

MainWindow::MainWindow()
{
    setPreferredSize( QSize( 1280, 800 ) );
    setTitle( tr("Tractor UI") );

    setupUI();
}

void MainWindow::setupUI()
{
    if (m_mainBox) {
        m_mainBox->deleteLater();
    }
    
    m_mainBox = new MainBox( contentItem() );
    connect( m_mainBox, &MainBox::languageChanged, this, &MainWindow::switchLanguage );
}

void MainWindow::rebuildUI()
{
    // Rebuild the entire UI
    setupUI();
    
    // Update window title
    setTitle( tr("Tractor UI") );
}

void MainWindow::switchLanguage( const QString& localeName )
{
    // Remove the old translator
    if (m_translator) {
        qApp->removeTranslator(m_translator);
        delete m_translator;
        m_translator = nullptr;
    }

    // Load the new translator
    m_translator = new QTranslator(this);
    QString qmFile = QString(":/translations/translations_%1.qm").arg(localeName);
    
    if (m_translator->load(qmFile))
    {
        qApp->installTranslator(m_translator);
        
        // Update window title
        setTitle( tr("Tractor UI") );
        
        // Rebuild the MainBox content with fade animation
        if (m_mainBox) {
            m_mainBox->rebuildUI();
        }
    }
}

#include "moc_MainWindow.cpp"

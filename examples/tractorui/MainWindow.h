/******************************************************************************
 * Copyright (C) 2021 Edelhirsch Software GmbH
 *           SPDX-License-Identifier: BSD-3-Clause
 *****************************************************************************/

#pragma once

#include <QskWindow.h>

#include <QTranslator>

class MainBox;

class MainWindow : public QskWindow
{
    Q_OBJECT

  public:
    MainWindow();

  public Q_SLOTS:
    void switchLanguage( const QString& localeName );

  private:
    void rebuildUI();
    void setupUI();

    QTranslator* m_translator = nullptr;
    MainBox* m_mainBox = nullptr;
};

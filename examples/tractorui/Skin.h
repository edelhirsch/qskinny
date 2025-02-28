/******************************************************************************
 * Copyright (C) 2021 Edelhirsch Software GmbH
 *           SPDX-License-Identifier: BSD-3-Clause
 *****************************************************************************/

#pragma once

#include <QskGradient.h>
#include <QskSkin.h>

class QskMaterial3Theme;

class Skin : public QskSkin
{
    Q_OBJECT

    using Inherited = QskSkin;

  public:
    enum GraphicRole
    {
        GraphicRoleOnSurfaceVariant,
        GraphicRolePrimary,
    };

    Skin( QObject* parent = nullptr );
    ~Skin() override;

  private:
    void initHints() override;

  private:
    void setupFonts();
    void setupGraphicFilters( const QskMaterial3Theme& );
    void setGraphicColor( GraphicRole, QRgb );

    class PrivateData;
    std::unique_ptr< PrivateData > m_data;
};

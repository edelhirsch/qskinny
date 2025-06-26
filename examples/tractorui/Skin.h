/******************************************************************************
 * Copyright (C) 2021 Edelhirsch Software GmbH
 *           SPDX-License-Identifier: BSD-3-Clause
 *****************************************************************************/

#pragma once

#include <QskGradient.h>
#include <QskSkin.h>

#include <material3/QskMaterial3Skin.h>

class TractorTheme;

class Skin : public QskSkin
{
    Q_OBJECT

    using Inherited = QskSkin;

  public:
    enum GraphicRole
    {
        GraphicRoleOnPrimaryContainer,
        GraphicRoleOnSurfaceVariant,
        GraphicRolePrimary,
        GraphicRolePrimaryBase,
    };

    Skin( QObject* parent = nullptr );
    Skin( const QskMaterial3Theme::BaseColors& baseColors, QObject* parent = nullptr );
    ~Skin() override;

    void setBaseColors( const QskMaterial3Theme::BaseColors& colors );

  private:
    void initHints() override;

  private:
    void setupFonts();
    void setupGraphicFilters( const TractorTheme& );
    void setGraphicColor( GraphicRole, QRgb );

    class PrivateData;
    std::unique_ptr< PrivateData > m_data;
};

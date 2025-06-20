/******************************************************************************
 * Copyright (C) The authors
 *           SPDX-License-Identifier: BSD-3-Clause
 *****************************************************************************/

#pragma once

#include <QskLinearBox.h>
#include <QskAnimator.h>

class HeaderElementsBackgroundBox : public QskBox
{
    Q_OBJECT

  public:
    QSK_SUBCONTROLS( Panel )

    HeaderElementsBackgroundBox( QQuickItem* parent );
};

class HeaderElementsBox : public QskLinearBox
{
    Q_OBJECT

  public:
    QSK_SUBCONTROLS( Panel )

    static constexpr QskAspect::Variation Left = QskAspect::Left;
    static constexpr QskAspect::Variation Right = QskAspect::Right;

    enum class Position
    {
        Left = Left,
        Right = Right,
    };

    HeaderElementsBox( Position pos, QQuickItem* parent );

    void updateLayout() override;
    QskAspect::Variation effectiveVariation() const override;

    class PrivateData;
    std::unique_ptr< PrivateData > m_data;
};

class MainBox : public QskLinearBox
{
    Q_OBJECT

  public:
    QSK_SUBCONTROLS( Panel )

    MainBox( QQuickItem* parent = nullptr );
    ~MainBox();

    void rebuildUI();

Q_SIGNALS:
    void languageChanged( const QString& localeName );

  protected:
    void keyPressEvent( QKeyEvent* ) override final;
    void gestureEvent( QskGestureEvent* ) override final;

  private:
    void setupHeaderBox();
    void setupContentBox();
    void setupSidebar();
    void setupTileArea();
    void setupCube();
    void startFadeAnimation();
    void performRebuild();

    class FadeAnimator;

    class PrivateData;
    std::unique_ptr< PrivateData > m_data;
    FadeAnimator* m_fadeAnimator = nullptr;
};

/******************************************************************************
 * Copyright (C) The authors
 *           SPDX-License-Identifier: BSD-3-Clause
 *****************************************************************************/

#pragma once

#include <QskBox.h>
#include <QskLabelData.h>
#include <QskPopup.h>
#include <QskPushButton.h>
#include <QskTextLabel.h>

class QskLinearBox;
class QskSegmentedBar;

class Button : public QskPushButton
{
    Q_OBJECT

  public:
    QSK_SUBCONTROLS( Panel, InnerPanel1, InnerPanel2, InnerPanel3, Icon, Text )

    static constexpr QskAspect::Variation Normal = QskAspect::NoVariation;
    static constexpr QskAspect::Variation Sidebar = QskAspect::Left;

    enum class Type
    {
        Normal = Normal,
        Sidebar = Sidebar,
    };

    Button( const QString& iconSource, QQuickItem* parent = nullptr );

    Type type() const;
    void setType( Type type );

  protected:
    QskAspect::Variation effectiveVariation() const override;

  private:
    Type m_type;
};

class SidebarOverlayButton : public QskPushButton
{
    Q_OBJECT

  public:
    QSK_SUBCONTROLS( Panel )

    SidebarOverlayButton( QQuickItem* parent );
};

class SidebarButtonPopup : public QskPopup
{
    Q_OBJECT

  public:
    QSK_SUBCONTROLS( Panel, Overlay )

    SidebarButtonPopup( Button* button, const QVector< QskLabelData >& options, int index = -1 );

    QRectF clipRect() const override;
    QskAspect fadingAspect() const override;

  Q_SIGNALS:
    void selectedIndexChanged( int index );

  protected:
    void updateNode( QSGNode* ) override;

  private:
    const Button* const m_button;
    QskLinearBox* m_linearBox;
    QskSegmentedBar* m_bar;
};

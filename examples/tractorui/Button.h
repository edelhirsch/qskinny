/******************************************************************************
 * Copyright (C) The authors
 *           SPDX-License-Identifier: BSD-3-Clause
 *****************************************************************************/

#pragma once

#include <QskBox.h>
#include <QskPushButton.h>
#include <QskTextLabel.h>

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

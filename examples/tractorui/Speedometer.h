/******************************************************************************
 * QSkinny - Copyright (C) The authors
 *           SPDX-License-Identifier: BSD-3-Clause
 *****************************************************************************/

#pragma once

#include <QskBoundedControl.h>

class Speedometer : public QskBoundedControl
{
    Q_OBJECT

  public:
    QSK_SUBCONTROLS( Panel1, Panel2, Panel3, ValueText, UnitText );

    Speedometer( QQuickItem* parent = nullptr );
    ~Speedometer();

    qreal value() const;
    void setValue( qreal value );

    QString unitString() const;
    void setUnitString( const QString& unitString );

  Q_SIGNALS:
    void valueChanged();
    void unitStringChanged();

  private:
    class PrivateData;
    std::unique_ptr< PrivateData > m_data;
};

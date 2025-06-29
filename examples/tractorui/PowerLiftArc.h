/******************************************************************************
 * QSkinny - Copyright (C) The authors
 *           SPDX-License-Identifier: BSD-3-Clause
 *****************************************************************************/

#pragma once

#include <QskBoundedInput.h>
#include <QskGraphicLabel.h>
#include <QskIntervalF.h>

class TractorLabel : public QskGraphicLabel
{
    Q_OBJECT

  public:
    QSK_SUBCONTROLS( Panel, Graphic )

    TractorLabel( const QString& url, QQuickItem* parent = nullptr );
};

class PowerLiftArc : public QskBoundedInput
{
    Q_OBJECT

  public:
    QSK_SUBCONTROLS( Boundaries, Groove, Fill, ProgrammedFill, MinHandle, MaxHandle, ValueHandle )

    static constexpr QskAspect::Variation Front = QskAspect::NoVariation;
    static constexpr QskAspect::Variation Back = QskAspect::Left;

    enum class Type
    {
        Front = Front,
        Back = Back
    };

    PowerLiftArc( Type type, QQuickItem* parent = nullptr );

    Type type() const;

    qreal value() const;
    void setValue( qreal value );

    qreal programmedValue() const;
    void setProgrammedValue( qreal value );

  public Q_SLOTS:
    void increment( qreal offset ) override;

  Q_SIGNALS:
    void valueChanged( qreal );
    void programmedValueChanged( qreal );

  protected:
    QskAspect::Variation effectiveVariation() const override;

  private:
    const Type m_type;
    qreal m_value;
    qreal m_programmedValue;
};

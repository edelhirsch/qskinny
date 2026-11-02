/******************************************************************************
 * QSkinny - Copyright (C) The authors
 *           SPDX-License-Identifier: BSD-3-Clause
 *****************************************************************************/

#pragma once

#include <QskBoundedValueInput.h>
#include <QskGraphicLabel.h>

class TractorLabel : public QskGraphicLabel
{
    Q_OBJECT

  public:
    QSK_SUBCONTROLS( Panel, Graphic )

    TractorLabel( const QString& url, QQuickItem* parent = nullptr );
};

/*
    An arc shaped value input, modelled after the iotdashboard "light intensity"
    control: the current value is shown as a conical fill along the groove and
    carries a draggable handle. ProgrammedFill is a secondary indicator for a
    target ( "programmed" ) value drawn underneath the live fill.

    value() / setValue() etc. are inherited from QskBoundedValueInput.
*/
class PowerLiftArc : public QskBoundedValueInput
{
    Q_OBJECT

    using Inherited = QskBoundedValueInput;

  public:
    QSK_SUBCONTROLS( Boundaries, Groove, Fill, ProgrammedFill, ValueHandle )
    QSK_STATES( Pressed )

    static constexpr QskAspect::Variation Front = QskAspect::NoVariation;
    static constexpr QskAspect::Variation Back = QskAspect::Left;

    enum class Type
    {
        Front = Front,
        Back = Back
    };

    PowerLiftArc( Type type, QQuickItem* parent = nullptr );

    Type type() const;

    bool isPressed() const;

    // the scene y coordinate of the value handle's centre, so a sibling can
    // line something up with the handle at any size
    qreal handleCenterSceneY() const;

    qreal programmedValue() const;
    void setProgrammedValue( qreal value );

  Q_SIGNALS:
    void programmedValueChanged( qreal );

  protected:
    QskAspect::Variation effectiveVariation() const override;

    void mousePressEvent( QMouseEvent* ) override;
    void mouseMoveEvent( QMouseEvent* ) override;
    void mouseReleaseEvent( QMouseEvent* ) override;

  private:
    // the value [0..1] corresponding to a point in local coordinates, or -1
    // when the point is not close enough to the groove arc
    qreal valueAtPoint( const QPointF& ) const;

    const Type m_type;
    qreal m_programmedValue;
};

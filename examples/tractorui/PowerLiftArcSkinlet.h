/******************************************************************************
 * Copyright (C) The authors
 *           SPDX-License-Identifier: BSD-3-Clause
 *****************************************************************************/

#pragma once

#include <QskSkinlet.h>

class PowerLiftArc;

class PowerLiftArcSkinlet : public QskSkinlet
{
        Q_GADGET

    public:
        enum NodeRole
        {
            BoundariesRole,
            GrooveShadowRole,
            GrooveRole,
            FillGlowRole,
            FillRole,
            ProgrammedFillRole,
            MinHandleRole,
            MaxHandleRole,
            ValueHandleRole,
        };

        Q_INVOKABLE PowerLiftArcSkinlet( QskSkin* = nullptr );

        QRectF subControlRect( const QskSkinnable*,
                               const QRectF&, QskAspect::Subcontrol ) const override;

        QSizeF sizeHint( const QskSkinnable*,
                         Qt::SizeHint, const QSizeF& ) const override;

    protected:
        QSGNode* updateSubNode( const QskSkinnable*,
                                quint8 nodeRole, QSGNode* ) const override;

    private:
        QSGNode* updateShadowNode( const PowerLiftArc* arc, QSGNode* node ) const;
        QSGNode* updateGlowNode( const PowerLiftArc* arc, QSGNode* node ) const;

        QskArcMetrics fillMetrics( const PowerLiftArc* arc ) const;
};

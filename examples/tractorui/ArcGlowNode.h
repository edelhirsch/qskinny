/******************************************************************************
 * QSkinny - Copyright (C) The authors
 *           SPDX-License-Identifier: BSD-3-Clause
 *****************************************************************************/

#ifndef ARC_GLOW_NODE_H
#define ARC_GLOW_NODE_H

#include "QskGlobal.h"
#include <qsgnode.h>

class QskArcMetrics;
class QskShadowMetrics;

class ArcGlowNodePrivate;

class ArcGlowNode : public QSGGeometryNode
{
  public:
    ArcGlowNode();
    ~ArcGlowNode() override;

    void setGlowData(
        const QRectF& rect,
        qreal spreadRadius,
        qreal blurRadius,
        qreal startAngle,
        qreal spanAngle,
        const QColor& color);

  private:
    void setBoundingRectangle(const QRectF&);

    Q_DECLARE_PRIVATE(ArcGlowNode)
};

#endif 
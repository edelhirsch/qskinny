/******************************************************************************
 * QSkinny - Copyright (C) The authors
 *           SPDX-License-Identifier: BSD-3-Clause
 *****************************************************************************/

#pragma once

#include <QskControl.h>

#include <memory>

/*
    The tractor with its movable front loader.

    The drawing is composed from three overlaid graphics: the static body
    ( including the mounting plate ), the boom + bucket, and the upper arm.
    The boom rotates about its hinge while the upper arm stays pinned to the
    plate and stretches to follow it, so the loader raises and lowers as one
    linkage. The position is driven through setLoaderAngle().
*/
class FrontPowerLiftImage : public QskControl
{
  public:
    FrontPowerLiftImage( QQuickItem* parent = nullptr );
    ~FrontPowerLiftImage() override;

    // 0 = fully raised ( the drawing's home position ), larger = lowered
    void setLoaderAngle( qreal degrees );

  protected:
    void updateLayout() override;

  private:
    class PrivateData;
    std::unique_ptr< PrivateData > m_data;
};

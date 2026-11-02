/******************************************************************************
 * QSkinny - Copyright (C) The authors
 *           SPDX-License-Identifier: BSD-3-Clause
 *****************************************************************************/

#pragma once

#include <QskControl.h>

#include <memory>

class PowerLiftArc;

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
    Q_OBJECT

  public:
    FrontPowerLiftImage( QQuickItem* parent = nullptr );
    ~FrontPowerLiftImage() override;

    // 0 = fully raised ( the drawing's home position ), larger = lowered
    void setLoaderAngle( qreal degrees );

    // couple the loader to an arc: the boom is swung so the bucket's lower edge
    // stays level with the arc's handle, recomputed from the live geometry so
    // it holds at any window size
    void setArc( const PowerLiftArc* );

  public Q_SLOTS:
    void updateFromArc();

  protected:
    void updateLayout() override;

  private:
    class PrivateData;
    std::unique_ptr< PrivateData > m_data;
};

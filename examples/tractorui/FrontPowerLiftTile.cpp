/******************************************************************************
 * QSkinny - Copyright (C) The authors
 *           SPDX-License-Identifier: BSD-3-Clause
 *****************************************************************************/

#include "FrontPowerLiftTile.h"

#include "FrontPowerLiftImage.h"
#include "PowerLiftArc.h"

#include <QskLinearBox.h>

FrontPowerLiftTile::FrontPowerLiftTile( QQuickItem* parent )
    : Tile( "front power lift", parent )
{
    auto* outerBox = new QskLinearBox( Qt::Horizontal, this );
    outerBox->setMargins( { 20, 20, 20, 20 } );
    outerBox->setSpacing( 35 );

    auto* image = new FrontPowerLiftImage( outerBox );

    auto* frontArc = new PowerLiftArc( PowerLiftArc::Type::Front, outerBox );

    // The loader follows the arc handle: the image swings the boom so the
    // bucket's lower edge stays level with the handle. This is recomputed from
    // the live geometry ( see FrontPowerLiftImage::updateFromArc ), so the two
    // stay aligned at any window size.
    image->setArc( frontArc );
    connect( frontArc, &PowerLiftArc::valueChanged, image, &FrontPowerLiftImage::updateFromArc );

    frontArc->setValue( 0.8 );
    frontArc->setProgrammedValue( 0.8 );

    setContent( outerBox );
}

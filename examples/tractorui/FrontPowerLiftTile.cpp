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

    // The loader is articulated and driven through FrontPowerLiftImage::setLoaderAngle(),
    // 0 = fully raised ( the drawing's home ), larger = lowered. Map the arc value
    // so the handle and the bucket are always at the same height: value 1 raises
    // the loader, value 0 lowers it by the full travel.
    constexpr qreal maxLoaderAngle = 45.0; // degrees of boom travel

    const auto applyValue = [ image ]( qreal value )
    {
        image->setLoaderAngle( ( 1.0 - value ) * maxLoaderAngle );
    };

    connect( frontArc, &PowerLiftArc::valueChanged, image, applyValue );

    frontArc->setValue( 0.8 );
    frontArc->setProgrammedValue( 0.8 );
    applyValue( frontArc->value() ); // initial sync

    setContent( outerBox );
}

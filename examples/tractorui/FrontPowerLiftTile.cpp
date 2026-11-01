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
    frontArc->setValue( 0.8 );
    frontArc->setProgrammedValue( 0.8 );

    // The loader is articulated and driven through FrontPowerLiftImage::setLoaderAngle(),
    // 0 = raised ( the drawing's home ), larger = lowered. Once the arc has a drag
    // interaction, hook it up, e.g.:
    //   connect( frontArc, &PowerLiftArc::valueChanged, image,
    //       [ image ]( qreal v ) { image->setLoaderAngle( ( 1.0 - v ) * 26.0 ); } );
    Q_UNUSED( image )

    setContent( outerBox );
}

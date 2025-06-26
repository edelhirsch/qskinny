/******************************************************************************
 * QSkinny - Copyright (C) The authors
 *           SPDX-License-Identifier: BSD-3-Clause
 *****************************************************************************/

#include "FrontPowerLiftTile.h"

#include "PowerLiftArc.h"
#include "QuickAccessButton.h"

#include <QskFontRole.h>
#include <QskGraphicLabel.h>
#include <QskGridBox.h>

FrontPowerLiftTile::FrontPowerLiftTile( QQuickItem* parent )
    : Tile( "front power lift", parent )
{
    auto* outerBox = new QskLinearBox( Qt::Horizontal, this );
    outerBox->setMargins( { 20, 20, 20, 20 } );
    outerBox->setSpacing( 35 );

    auto* tractor = new TractorLabel( "tractor", outerBox );
    tractor->setFillMode( QskGraphicLabel::Stretch );
    tractor->setSizePolicy( QskSizePolicy::Expanding, QskSizePolicy::Preferred );

    auto* frontArc = new PowerLiftArc( PowerLiftArc::Type::Front, outerBox );
    frontArc->setValue( 0.8 );
    frontArc->setProgrammedValue( 0.8 );

    setContent( outerBox );
}

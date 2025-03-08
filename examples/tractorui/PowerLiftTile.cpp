/******************************************************************************
 * QSkinny - Copyright (C) The authors
 *           SPDX-License-Identifier: BSD-3-Clause
 *****************************************************************************/

#include "PowerLiftTile.h"

#include "PowerLiftArc.h"
#include "QuickAccessButton.h"

#include <QskGraphicLabel.h>

PowerLiftTile::PowerLiftTile( QQuickItem* parent )
    : Tile( "power lift", parent )
{
    auto* outerBox = new QskLinearBox( Qt::Horizontal, this );

    auto* arcsBox = new QskLinearBox( Qt::Horizontal, outerBox );
    arcsBox->setMargins( 0, 30, 0, 30 );
    auto* backArc = new PowerLiftArc( PowerLiftArc::Type::Back, arcsBox );
    backArc->setValue( 0.3 );

    auto* tractor = new TractorLabel( "tractor", arcsBox );
    tractor->setMargins( { 0, 20, 0, 0 } );
    tractor->setSizePolicy( QskSizePolicy::Constrained, QskSizePolicy::Preferred );

    auto* frontArc = new PowerLiftArc( PowerLiftArc::Type::Front, arcsBox );
    frontArc->setValue( 0.8 );

    auto* buttonsBox = new QskLinearBox( Qt::Vertical, outerBox );
    buttonsBox->setSizePolicy( Qt::Horizontal, QskSizePolicy::Fixed );
    buttonsBox->setMargins( 10 );
    buttonsBox->setSpacing( 10 );

    auto* velocityButton = new QuickAccessButton( "clock", "velocity", buttonsBox );
    auto* weightsButton = new QuickAccessButton( "weight-hanging", "weights", buttonsBox );

    setContent( outerBox );
}

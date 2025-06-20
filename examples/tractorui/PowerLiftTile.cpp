/******************************************************************************
 * QSkinny - Copyright (C) The authors
 *           SPDX-License-Identifier: BSD-3-Clause
 *****************************************************************************/

#include "PowerLiftTile.h"

#include "PowerLiftArc.h"
#include "QuickAccessButton.h"

#include <QskFontRole.h>
#include <QskGraphicLabel.h>
#include <QskGridBox.h>

PowerLiftTile::PowerLiftTile( QQuickItem* parent )
    : Tile( "power lift", parent )
{
    auto* outerBox = new QskLinearBox( Qt::Horizontal, this );
    // outerBox->setMargins( { 5, 5, 5, 10 } );

    auto* arcsBox = new QskGridBox( outerBox );
    arcsBox->setMargins( 10, 0, 10, 20 );
    arcsBox->setSpacing( Qt::Vertical, 10 );
    auto* backArc = new PowerLiftArc( PowerLiftArc::Type::Back, arcsBox );
    backArc->setValue( 0.3 );
    backArc->setProgrammedValue( 0.3 );
    arcsBox->addItem( backArc, 0, 0 );

    auto* tractor = new TractorLabel( "tractor", arcsBox );
    tractor->setMargins( { 0, 20, 0, 0 } );
    tractor->setSizePolicy( QskSizePolicy::Constrained, QskSizePolicy::Preferred );
    arcsBox->addItem( tractor, 0, 1 );

    auto* frontArc = new PowerLiftArc( PowerLiftArc::Type::Front, arcsBox );
    frontArc->setValue( 0.8 );
    frontArc->setProgrammedValue( 0.8 );
    arcsBox->addItem( frontArc, 0, 2 );

    auto* backLabel = new QskTextLabel( tr("back"), arcsBox );
    backLabel->setSizePolicy( Qt::Horizontal, QskSizePolicy::Fixed );
    backLabel->setFontRole( QskFontRole::Title );
    arcsBox->addItem( backLabel, 1, 0 );

    auto* frontLabel = new QskTextLabel( tr("front"), arcsBox );
    frontLabel->setSizePolicy( Qt::Horizontal, QskSizePolicy::Fixed );
    frontLabel->setFontRole( QskFontRole::Title );
    arcsBox->addItem( frontLabel, 1, 2 );

    auto* buttonsBox = new QskLinearBox( Qt::Vertical, outerBox );
    buttonsBox->setSizePolicy( Qt::Horizontal, QskSizePolicy::Fixed );
    buttonsBox->setMargins( 10 );
    buttonsBox->setSpacing( 10 );

    auto* velocityButton = new QuickAccessButton( "clock", tr("velocity"), buttonsBox );
    auto* weightsButton = new QuickAccessButton( "weight-hanging", tr("weights"), buttonsBox );

    setContent( outerBox );
}

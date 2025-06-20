/******************************************************************************
 * QSkinny - Copyright (C) The authors
 *           SPDX-License-Identifier: BSD-3-Clause
 *****************************************************************************/

#include "EngineTile.h"

#include "Button.h"
#include "Speedometer.h"
#include "Switch.h"

#include <QskFontRole.h>

EngineTile::EngineTile( QQuickItem* parent )
    : Tile( "engine", parent )
{
    auto* box = new QskLinearBox( Qt::Horizontal, this );

    auto* speedBox = new QskLinearBox( Qt::Vertical, box );
    speedBox->setSpacing( 20 );
    speedBox->setExtraSpacingAt( Qt::TopEdge | Qt::BottomEdge );
    speedBox->setDefaultAlignment( Qt::AlignCenter );

    auto* speed = new Speedometer( speedBox );
    speed->setBoundaries( 0, 70 );
    speed->setValue( 53 );
    speed->setUnitString( tr("km/h") );

    auto* speedLabel = new QskTextLabel( tr("speed"), speedBox );
    speedLabel->setAlignment( Qt::AlignCenter );
    speedLabel->setFontRole( QskFontRole::Title );


    auto* driveBox = new QskLinearBox( Qt::Vertical, box );
    driveBox->setSpacing( 20 );
    driveBox->setExtraSpacingAt( Qt::TopEdge | Qt::BottomEdge );
    driveBox->setDefaultAlignment( Qt::AlignCenter );

    auto* drive = new Speedometer( driveBox );
    drive->setShowIntensity( true );
    drive->setBoundaries( 0, 80 );
    drive->setValue( 25 );
    drive->setUnitString( tr("*100 rpm") );

    auto* driveLabel = new QskTextLabel( tr("drive"), driveBox );
    driveLabel->setAlignment( Qt::AlignCenter );
    driveLabel->setFontRole( QskFontRole::Title );

    auto* roadAndCruiseControlBox = new QskLinearBox( Qt::Vertical, box );
    roadAndCruiseControlBox->setDefaultAlignment( Qt::AlignCenter );
    roadAndCruiseControlBox->setSizePolicy( Qt::Horizontal, QskSizePolicy::Fixed );
    roadAndCruiseControlBox->setMargins( 20 );
    roadAndCruiseControlBox->setSpacing( 20 );

    auto* roadModeSwitch = new Switch( "road", roadAndCruiseControlBox );
    auto* cruiseControlButton = new Button( "arrow-down", roadAndCruiseControlBox );
    cruiseControlButton->setText( tr("cruise control") );

    setContent( box );
}

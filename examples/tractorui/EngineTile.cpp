/******************************************************************************
 * QSkinny - Copyright (C) The authors
 *           SPDX-License-Identifier: BSD-3-Clause
 *****************************************************************************/

#include "EngineTile.h"

#include "Speedometer.h"

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
    speed->setValue( 35 );
    speed->setUnitString( "km/h" );

    auto* speedLabel = new QskTextLabel( "speed", speedBox );
    speedLabel->setAlignment( Qt::AlignCenter );
    speedLabel->setFontRole( QskFontRole::Title );


    auto* driveBox = new QskLinearBox( Qt::Vertical, box );
    driveBox->setSpacing( 20 );
    driveBox->setExtraSpacingAt( Qt::TopEdge | Qt::BottomEdge );
    driveBox->setDefaultAlignment( Qt::AlignCenter );

    auto* drive = new Speedometer( driveBox );
    drive->setValue( 25 );
    drive->setUnitString( "*100 rpm" );

    auto* driveLabel = new QskTextLabel( "drive", driveBox );
    driveLabel->setAlignment( Qt::AlignCenter );
    driveLabel->setFontRole( QskFontRole::Title );

    setContent( box );
}

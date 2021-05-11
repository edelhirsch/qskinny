/******************************************************************************
 * Copyright (C) 2021 Edelhirsch Software GmbH
 * This file may be used under the terms of the 3-clause BSD License
 *****************************************************************************/

#include "Skin.h"

#include "ControlNew.h"
#include "ControlNewSkinlet.h"
#include "ControlOld.h"
#include "ControlOldSkinlet.h"

#include <QskBoxBorderMetrics.h>
#include <QskBoxBorderColors.h>
#include <QskFunctions.h>
#include <QskSkinHintTableEditor.h>
#include <QskTextLabel.h>

Skin::Skin( QObject* parent ) : QskSkin( parent )
{
    declareSkinlet<ControlNew, ControlNewSkinlet>();
    declareSkinlet<ControlOld, ControlOldSkinlet>();

    initHints();
}

Skin::~Skin()
{
}

void Skin::initHints()
{
    QskSkinHintTableEditor ed( &hintTable() );

    ed.setGradient( ControlOld::Subcontrol1, Qt::red );
    ed.setGradient( ControlOld::Subcontrol2, Qt::green );
    ed.setGradient( ControlOld::Subcontrol3, Qt::blue );
    ed.setGradient( ControlOld::Subcontrol4, Qt::yellow );
}

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
#include <QskPalette.h>
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

    // old style: Define colors per subcontrol
    ed.setGradient( ControlOld::Subcontrol1, Qt::red );
    ed.setGradient( ControlOld::Subcontrol2, Qt::green );
    ed.setGradient( ControlOld::Subcontrol3, Qt::blue );
    ed.setGradient( ControlOld::Subcontrol4, Qt::yellow );

    ed.setColor( ControlOld::Text1, Qt::white );
    ed.setColor( ControlOld::Text2, Qt::black );
    ed.setColor( ControlOld::Text3, Qt::white );
    ed.setColor( ControlOld::Text4, Qt::black );

    // new style: Define colors per style / palette:
    ed.setGradient( QskPalette::Primary, Qt::red );
    ed.setGradient( QskPalette::Secondary, Qt::green );
    ed.setGradient( QskPalette::Background, Qt::blue );
    ed.setGradient( QskPalette::Surface, Qt::yellow );

    ed.setColor( QskPalette::OnPrimary, Qt::white );
    ed.setColor( QskPalette::OnSecondary, Qt::black );
    ed.setColor( QskPalette::OnBackground, Qt::white );
    ed.setColor( QskPalette::OnSurface, Qt::black );
}

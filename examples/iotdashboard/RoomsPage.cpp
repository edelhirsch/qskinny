/******************************************************************************
 * Copyright (C) 2021 Edelhirsch Software GmbH
 * This file may be used under the terms of the 3-clause BSD License
 *****************************************************************************/

#include "RoomsPage.h"

#include "Box.h"
#include "BoxWithButtons.h"
#include "GridBox.h"
#include "LightDisplay.h"
#include "MyDevices.h"
#include "PieChart.h"
#include "TopBar.h"
#include "UsageBox.h"
#include "UsageDiagram.h"

#include <QskBoxBorderColors.h>
#include <QskBoxBorderMetrics.h>
#include <QskBoxShapeMetrics.h>
#include <QskGridBox.h>
#include <QskSetup.h>
#include <QskSkin.h>
#include <QskTextLabel.h>
#include <QskQuick.h>

#include <QTimer>

QSK_SUBCONTROL( RoomsPage, Panel )

namespace
{
    class IndoorTemperature : public BoxWithButtons
    {
      public:
        IndoorTemperature( QQuickItem* parent = nullptr )
            : BoxWithButtons( "Indoor Temperature", "+24", true, parent )
        {
        }
    };

    class Humidity : public BoxWithButtons
    {
      public:
        Humidity( QQuickItem* parent = nullptr )
            : BoxWithButtons( "Humidity", "30%", false, parent )
        {
        }
    };

    class LightIntensity : public Box
    {
      public:
        LightIntensity( QQuickItem* parent = nullptr )
            : Box( "Light intensity", parent )
        {
            addSpacer( 5 );
            auto* lightDisplay = new LightDisplay( this );
            lightDisplay->setValue( 50.0 );
        }
    };
}

RoomsPage::RoomsPage( QQuickItem* parent )
    : QskLinearBox( Qt::Vertical, parent )
{
    setPanel( true );
    setSubcontrolProxy( QskBox::Panel, RoomsPage::Panel );

    setAutoAddChildren( false );
    setSizePolicy( QskSizePolicy::Expanding, QskSizePolicy::Expanding );
    setDefaultAlignment( Qt::AlignTop );
    setSpacing( 24 );

    auto gridBox = new MainContentGridBox();
    gridBox->setPadding( 30 );
    gridBox->setPanel( true );
    gridBox->setSpacing( 15 );

    gridBox->addItem( new UsageDiagramBox( "Living Room" ), 0, 0 );
    gridBox->addItem( new UsageDiagramBox( "Bedroom" ), 0, 1 );
    gridBox->addItem( new UsageDiagramBox( "Bathroom" ), 1, 0 );
    gridBox->addItem( new UsageDiagramBox( "Kitchen" ), 1, 1 );

    addItem( gridBox );
}

#include "moc_RoomsPage.cpp"

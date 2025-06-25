/******************************************************************************
 * QSkinny - Copyright (C) The authors
 *           SPDX-License-Identifier: BSD-3-Clause
 *****************************************************************************/

#include "CabinTile.h"

#include "Button.h"
#include "Switch.h"

#include <QskFontRole.h>
#include <QskFunctions.h>
#include <QskGridBox.h>
#include <QskSlider.h>

namespace
{
    // ### We could also make a class out of this
    void setColumnWidths( const QVector< LabeledSlider* >& sliders )
    {
        qreal w1 = 0, w2 = 0;

        for( auto* s : sliders )
        {
            auto vMin = QString::number( s->slider()->minimum(), 'f', 0 );
            auto vMax = QString::number( s->slider()->maximum(), 'f', 0 );

            const auto f = s->effectiveFont( SliderLabel::Text );
            w1 = qMax( w1, qskHorizontalAdvance( f, vMin ) );
            w1 = qMax( w1, qskHorizontalAdvance( f, vMax ) );

            w2 = qMax( w2, qskHorizontalAdvance( f, s->unitString() ) );
        }

        for( auto* s : sliders )
        {
            s->setColumnFixedWidth( 1, w1 + 15 );
            s->setColumnFixedWidth( 2, w2 );
        }
    }
}

QSK_SUBCONTROL( SliderLabel, Panel )
QSK_SUBCONTROL( SliderLabel, Text )

SliderLabel::SliderLabel( const QString& text, QQuickItem* parent )
    : QskTextLabel( text, parent )
{
    setSubcontrolProxy( QskTextLabel::Panel, Panel );
    setSubcontrolProxy( QskTextLabel::Text, Text );
}

LabeledSlider::LabeledSlider( const QString& labelText, UnitConversion::UnitType unitType, const QString& explicitUnitString, QQuickItem* parent )
    : QskGridBox( parent )
    , UnitConversion( unitType )
{
    setDefaultAlignment( Qt::AlignCenter );
    setSpacing( Qt::Horizontal, 10 );
    setSizePolicy( Qt::Vertical, QskSizePolicy::Fixed );

    if( unit() == UnitConversion::Unit::None )
    {
        setExplicitUnitString( explicitUnitString );
    }

    m_slider = new QskSlider( Qt::Horizontal, this );

    auto* valueLabel = new SliderLabel( m_slider->valueText(), this );
    valueLabel->setSizePolicy( Qt::Horizontal, QskSizePolicy::Fixed );

    connect( m_slider, &QskSlider::valueChanged, this, [this, valueLabel]()
    {
        valueLabel->setText( QString::number( convertedValue( m_slider->value() ), 'f', 0 ) );
    } );

    auto* unitLabel = new SliderLabel( unitString(), this );
    unitLabel->setSizePolicy( Qt::Horizontal, QskSizePolicy::Fixed );
    unitLabel->setAlignmentHint( SliderLabel::Text, Qt::AlignLeft | Qt::AlignVCenter );

    auto* label = new QskTextLabel( labelText, this );
    label->setSizePolicy( Qt::Horizontal, QskSizePolicy::Fixed );
    label->setFontRole( QskFontRole::Title );

    addItem( m_slider, 0, 0, Qt::AlignCenter );
    addItem( valueLabel, 0, 1, Qt::AlignVCenter | Qt::AlignRight );
    addItem( unitLabel, 0, 2, Qt::AlignVCenter | Qt::AlignLeft );
    addItem( label, 1, 0, Qt::AlignCenter );
}

QskSlider* LabeledSlider::slider()
{
    return m_slider;
}

QString LabeledSlider::valueText() const
{
    return QString::number( convertedValue( m_slider->value() ), 'f', 0 );
}

CabinTile::CabinTile( QQuickItem* parent )
    : Tile( "cabin", parent )
{
    auto* outerBox = new QskLinearBox( Qt::Horizontal, this );
    outerBox->setMargins( 10 ); // ### move this to Tile?
    outerBox->setSpacing( 25 );

    auto* slidersBox = new QskLinearBox( Qt::Vertical, outerBox );

    auto* temperatureSlider = new LabeledSlider( tr("temperature"), UnitConversion::UnitType::Temperature, tr("°C"), slidersBox );
    temperatureSlider->slider()->setBoundaries( 15, 30 );
    temperatureSlider->slider()->setValue( 22 );

    auto* durationSlider = new LabeledSlider( tr("duration"), UnitConversion::UnitType::None, tr("min"), slidersBox );
    durationSlider->slider()->setBoundaries( 1, 60 );
    durationSlider->slider()->setValue( 30 );

    auto* fanSpeedSlider = new LabeledSlider( tr("fan speed"), UnitConversion::UnitType::None, QString(), slidersBox );
    fanSpeedSlider->slider()->setBoundaries( 0, 5 );
    fanSpeedSlider->slider()->setValue( 3 );

    const QVector< LabeledSlider* > sliders = { temperatureSlider, durationSlider, fanSpeedSlider };
    setColumnWidths( sliders );

    auto* buttonsBox = new QskLinearBox( Qt::Vertical, outerBox );
    buttonsBox->setSizePolicy( Qt::Horizontal, QskSizePolicy::Fixed );
    buttonsBox->setDefaultAlignment( Qt::AlignCenter );

    auto* ecoModeSwitch = new Switch( "leaf", buttonsBox );

    auto* maxButton = new Button( "wind", buttonsBox );
    maxButton->setText( tr("max") );

    setContent( outerBox );
}

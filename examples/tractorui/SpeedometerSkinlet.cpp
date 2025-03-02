/******************************************************************************
 * Copyright (C) The authors
 *           SPDX-License-Identifier: BSD-3-Clause
 *****************************************************************************/

#include "SpeedometerSkinlet.h"

#include "Speedometer.h"

#include <QskFunctions.h>
#include <QskGraphic.h>

#include <QFontMetricsF>

using Q = Speedometer;

SpeedometerSkinlet::SpeedometerSkinlet( QskSkin* skin )
    : QskSkinlet( skin )
{
    setNodeRoles(
    {
        Panel1Role,
        Panel2Role,
        Panel3Role,
        ValueTextRole,
        UnitTextRole,
    } );
}

QRectF SpeedometerSkinlet::subControlRect( const QskSkinnable* skinnable,
                                       const QRectF& contentsRect, QskAspect::Subcontrol subControl ) const
{
    const auto q = static_cast< const Q* >( skinnable );

    if( subControl == Q::Panel1
        || subControl == Q::Panel2
        || subControl == Q::Panel3 )
    {
        const auto s = q->strutSizeHint( subControl );
        QRectF r( { 0, 0 }, s );
        r.moveCenter( contentsRect.center() );

        return r;
    }

    if( subControl == Q::ValueText
        || subControl == Q::UnitText )
    {
        const auto f1 = q->effectiveFont( Q::ValueText );
        const auto f2 = q->effectiveFont( Q::UnitText );
        const QFontMetricsF fm1( f1 );
        const QFontMetricsF fm2( f2 );

        const auto w = qMax( qskHorizontalAdvance( f1, QString::number( q->value(), 'f', 0 ) ),
            qskHorizontalAdvance( f2, q->unitString() ) );
        const auto h1 = fm1.height();
        const auto h2 = fm2.height();

        QRectF r( 0, 0, w, h1 + h2 );
        r.moveCenter( contentsRect.center() );

        if( subControl == Q::ValueText )
        {
            r.setBottom( r.bottom() - h2 );
        }
        else
        {
            r.setTop( r.top() + h1 );
        }

        return r;
    }

    return QskSkinlet::subControlRect( skinnable, contentsRect, subControl );
}

QSizeF SpeedometerSkinlet::sizeHint( const QskSkinnable* skinnable, Qt::SizeHint, const QSizeF& ) const
{
    const auto q = static_cast< const Q* >( skinnable );

    return q->strutSizeHint( Q::Panel1 );
}

QSGNode* SpeedometerSkinlet::updateSubNode( const QskSkinnable* skinnable, quint8 nodeRole, QSGNode* node ) const
{
    const auto q = static_cast< const Q* >( skinnable );

    switch( nodeRole )
    {
        case Panel1Role:
        {
            return updateBoxNode( q, node, Q::Panel1 );
        }
        case Panel2Role:
        {
            return updateBoxNode( q, node, Q::Panel2 );
        }
        case Panel3Role:
        {
            return updateBoxNode( q, node, Q::Panel3 );
        }
        case ValueTextRole:
        {
            const auto s = QString::number( q->value(), 'f', 0 );
            return QskSkinlet::updateTextNode( q, node, s, Q::ValueText );
        }
        case UnitTextRole:
        {
            return QskSkinlet::updateTextNode( q, node, q->unitString(), Q::UnitText );
        }
    }

    return QskSkinlet::updateSubNode( skinnable, nodeRole, node );
}

#include "moc_SpeedometerSkinlet.cpp"

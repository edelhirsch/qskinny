/******************************************************************************
 * QSkinny - Copyright (C) 2016 Uwe Rathmann
 * This file may be used under the terms of the QSkinny License, Version 1.0
 *****************************************************************************/

#include "QskArcBorderMetrics.h"

#include <qhashfunctions.h>
#include <qvariant.h>

static void qskRegisterArcBorderMetrics()
{
    qRegisterMetaType< QskArcBorderMetrics >();

    QMetaType::registerConverter< int, QskArcBorderMetrics >(
        []( int width ) { return QskArcBorderMetrics( width ); } );

    QMetaType::registerConverter< qreal, QskArcBorderMetrics >(
        []( qreal width ) { return QskArcBorderMetrics( width ); } );
}

Q_CONSTRUCTOR_FUNCTION( qskRegisterArcBorderMetrics )

// copied from QskMargins.cpp, we should unify this somehwere:
static inline qreal qskInterpolated( qreal from, qreal to, qreal ratio )
{
    return from + ( to - from ) * ratio;
}

static inline qreal qskAbsoluted( qreal length, qreal percentage )
{
    // 100% means -> 0.5 of length
    percentage = qBound( 0.0, percentage, 100.0 );
    return percentage / 100.0 * 0.5 * length;
}

void QskArcBorderMetrics::setSizeMode( Qt::SizeMode sizeMode ) noexcept
{
    m_sizeMode = sizeMode;
}

void QskArcBorderMetrics::setWidth( Qsk::ArcPosition position, qreal width ) noexcept
{
    m_widths[ position ] =  qMax( width, 0.0 );
}

void QskArcBorderMetrics::setWidths( qreal width ) noexcept
{
    m_widths[0] = m_widths[1] = width;
}

void QskArcBorderMetrics::setWidths( qreal outer, qreal inner ) noexcept
{
    m_widths[0] = outer;
    m_widths[1] = inner;
}

void QskArcBorderMetrics::setOuterWidth( qreal width ) noexcept
{
    m_widths[0] = width;
}

void QskArcBorderMetrics::setInnerWidth( qreal width ) noexcept
{
    m_widths[1] = width;
}

QskArcBorderMetrics QskArcBorderMetrics::interpolated(
    const QskArcBorderMetrics& to, qreal ratio ) const noexcept
{
    if ( ( *this == to ) || ( m_sizeMode != to.m_sizeMode ) )
        return to;

    const qreal outerWidth = qskInterpolated( m_widths[0], to.m_widths[0], ratio );
    const qreal innerWidth = qskInterpolated( m_widths[1], to.m_widths[1], ratio );
    return QskArcBorderMetrics( outerWidth, innerWidth, m_sizeMode );
}

QVariant QskArcBorderMetrics::interpolate(
    const QskArcBorderMetrics& from, const QskArcBorderMetrics& to,
    qreal progress )
{
    return QVariant::fromValue( from.interpolated( to, progress ) );
}

QskArcBorderMetrics QskArcBorderMetrics::toAbsolute( const QSizeF& size ) const noexcept
{
    if ( m_sizeMode != Qt::RelativeSize )
        return *this;

    QskArcBorderMetrics absoluted = *this;

    auto& outerWidth = absoluted.m_widths[0];
    auto& innerWidth = absoluted.m_widths[1];

    if ( size.isEmpty() )
    {
        outerWidth = 0.0;
        innerWidth = 0.0;
    }
    else
    {
        // for now we just use the width:
        outerWidth = qskAbsoluted( size.width(), outerWidth );
        innerWidth = qskAbsoluted( size.width(), innerWidth );
    }

    absoluted.m_sizeMode = Qt::AbsoluteSize;

    return absoluted;
}

uint QskArcBorderMetrics::hash( uint seed ) const noexcept
{
    uint hash = qHash( m_widths[0], seed );
    hash = qHash( m_widths[1], hash );

    const int mode = m_sizeMode;
    return qHashBits( &mode, sizeof( mode ), hash );
}

#ifndef QT_NO_DEBUG_STREAM

#include <qdebug.h>

QDebug operator<<( QDebug debug, const QskArcBorderMetrics& metrics )
{
    QDebugStateSaver saver( debug );
    debug.nospace();

    debug << "ArcBorder" << '(';
    debug << metrics.sizeMode();
    debug << ", outer width:" << metrics.width( Qsk::Outer );
    debug << ", inner width:" << metrics.width( Qsk::Inner );
    debug << ')';

    return debug;
}

#endif

#include "moc_QskArcBorderMetrics.cpp"

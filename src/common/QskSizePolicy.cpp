/******************************************************************************
 * QSkinny - Copyright (C) 2016 Uwe Rathmann
 * This file may be used under the terms of the QSkinny License, Version 1.0
 *****************************************************************************/

#include "QskSizePolicy.h"

#ifndef QT_NO_DEBUG
#include <qdebug.h>
#endif

QskSizePolicy::Policy QskSizePolicy::policy( Qt::Orientation orientation ) const
{
    return ( orientation == Qt::Horizontal )
           ? horizontalPolicy() : verticalPolicy();
}

void QskSizePolicy::setPolicy( Qt::Orientation orientation, Policy policy )
{
    if ( orientation == Qt::Horizontal )
        setHorizontalPolicy( policy );
    else
        setVerticalPolicy( policy );
}

QskSizePolicy::ConstraintType QskSizePolicy::constraintType() const
{
    constexpr unsigned char mask = IgnoreFlag | ConstrainedFlag;

#ifndef QT_NO_DEBUG
    if ( ( ( m_horizontalPolicy & mask ) == mask )
        || ( ( m_verticalPolicy & mask ) == mask ) )
    {
        qWarning() << "invalid policy having IgnoreFlag and ConstrainedFlag";
    }
#endif

    if ( ( m_horizontalPolicy & mask ) == ConstrainedFlag )
        return QskSizePolicy::WidthForHeight;

    if ( ( m_verticalPolicy & mask ) == ConstrainedFlag )
        return QskSizePolicy::HeightForWidth;
        
    return QskSizePolicy::Unconstrained;
}   

Qt::SizeHint QskSizePolicy::effectiveSizeHintType(
    Qt::SizeHint which, Qt::Orientation orientation ) const
{
    const auto policy = ( orientation == Qt::Horizontal )
        ? horizontalPolicy() : verticalPolicy();

    if ( which == Qt::MinimumSize )
    {
        if ( !( policy & ShrinkFlag ) )
            return Qt::PreferredSize;
    }
    else if ( which == Qt::MaximumSize )
    {
        if ( !( policy & ( GrowFlag | ExpandFlag ) ) )
            return Qt::PreferredSize;
    }

    return which;
}

void QskSizePolicy::transpose()
{
    std::swap( m_horizontalPolicy, m_verticalPolicy );
}

QskSizePolicy QskSizePolicy::transposed() const
{
    return QskSizePolicy( verticalPolicy(), horizontalPolicy() );
}

#ifndef QT_NO_DEBUG_STREAM

#include <qdebug.h>

QDebug operator<<( QDebug debug, const QskSizePolicy& policy )
{
    QDebugStateSaver saver( debug );
    debug.nospace();
    debug << "SizePolicy" << '(';
    debug << policy.horizontalPolicy() << ", " << policy.verticalPolicy();
    debug << ')';

    return debug;
}

#endif

#include "moc_QskSizePolicy.cpp"

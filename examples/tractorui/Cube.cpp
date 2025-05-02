/******************************************************************************
 * Copyright (C) The authors
 *           SPDX-License-Identifier: BSD-3-Clause
 *****************************************************************************/

#include "Cube.h"

#include <QskPanGestureRecognizer.h>
#include <QskStackBoxAnimator.h>

#include <QTimer>

namespace
{
    class PanRecognizer final : public QskPanGestureRecognizer
    {
      public:
        PanRecognizer( QskControl* mainItem )
            : QskPanGestureRecognizer( mainItem )
        {
            setOrientations( Qt::Horizontal | Qt::Vertical );
            setMinDistance( 50 );
            setTimeout( 100 );

            setWatchedItem( mainItem );
        }
    };
}

QPair< Cube::Position, Cube::Edge > Cube::s_neighbors[ Cube::NumPositions ][ Cube::NumEdges ] =
{
    // neighbors of Left side:
    {
        { Cube::BackPos, Cube::BottomEdge }, // going Left
        { Cube::FrontPos, Cube::BottomEdge }, // going Right
        { Cube::TopPos, Cube::LeftEdge }, // going Top
        { Cube::BottomPos, Cube::RightEdge } // going Bottom
    },

    // Right:
    {
        { Cube::FrontPos, Cube::BottomEdge },
        { Cube::BackPos, Cube::BottomEdge },
        { Cube::TopPos, Cube::RightEdge },
        { Cube::BottomPos, Cube::LeftEdge }
    },

    // Top:
    {
        { Cube::LeftPos, Cube::RightEdge },
        { Cube::RightPos, Cube::LeftEdge },
        { Cube::BackPos, Cube::TopEdge },
        { Cube::FrontPos, Cube::BottomEdge }
    },

    // Bottom:
    {
        { Cube::LeftPos, Cube::LeftEdge },
        { Cube::RightPos, Cube::RightEdge },
        { Cube::FrontPos, Cube::BottomEdge },
        { Cube::BackPos, Cube::TopEdge }
    },

    // Front:
    {
        { Cube::LeftPos, Cube::BottomEdge },
        { Cube::RightPos, Cube::BottomEdge },
        { Cube::TopPos, Cube::BottomEdge },
        { Cube::BottomPos, Cube::BottomEdge }
    },

    // Back:
    {
        { Cube::RightPos, Cube::BottomEdge },
        { Cube::LeftPos, Cube::BottomEdge },
        { Cube::TopPos, Cube::TopEdge },
        { Cube::BottomPos, Cube::BottomEdge }
    }
};

Cube::Edge Cube::s_edgeTransformations[ Cube::NumEdges ][ Cube::NumEdges ] =
{
    // current edge is LeftEdge:
    { Cube::TopEdge, // Left
      Cube::BottomEdge, // Right
      Cube::RightEdge, // Top
      Cube::LeftEdge }, // Bottom

    // Right:
    { Cube::BottomEdge,
      Cube::TopEdge,
      Cube::LeftEdge,
      Cube::RightEdge },

    // Top:
    { Cube::RightEdge,
      Cube::LeftEdge,
      Cube::BottomEdge,
      Cube::TopEdge },

    // Bottom:
    { Cube::LeftEdge,
      Cube::RightEdge,
      Cube::TopEdge,
      Cube::BottomEdge }
};

Cube::Cube( QQuickItem* parent )
    : QskStackBox( false, parent )
    , m_destination( FrontPos )
    , m_currentEdge( BottomEdge )
    , m_isIntermediateHop( false )
{
    // The code below covers the case where we need 2 cube movements to get
    // to the desired position.
    // We use transientIndexChanged here to be sure to start a new transition
    // at the end; indexChanged doesn't work here.

    connect( this, &QskStackBox::transientIndexChanged, this, [ this ]( qreal position )
    {
        const bool animationIsFinished = ( position == qFloor( position ) );

        if( animationIsFinished && position != m_destination )
        {
            QTimer::singleShot( 0, this, [this]()
            {
                m_isIntermediateHop = true;
                switchToPosition( m_destination );
            } );
        }
    } );

    QTimer::singleShot( 0, this, [this]()
    {
        Q_EMIT cubeIndexChanged( m_destination );
    } );
}

void Cube::doSwitch( Qsk::Direction direction, Position position )
{
    using Animator = QskStackBoxAnimator4;

    auto animator = qobject_cast< Animator* >( this->animator() );

    if ( animator == nullptr )
    {
        animator = new Animator( this );
        animator->setDuration( 1000 );
        setAnimator( animator );
    }

    if( position == m_destination && !m_isIntermediateHop ) // 1 hop
    {
        animator->setEasingCurve( QEasingCurve::InOutQuad );
    }
    else if( !m_isIntermediateHop ) // 1st of 2 hops
    {
        animator->setEasingCurve( QEasingCurve::InQuad );
    }
    else // 2nd of 2 hops
    {
        animator->setEasingCurve( QEasingCurve::OutQuad );
        m_isIntermediateHop = false;
    }

    const auto orientation = ( direction == Qsk::LeftToRight || direction == Qsk::RightToLeft )
            ? Qt::Horizontal : Qt::Vertical;
    animator->setOrientation( orientation );

    const bool inverted = ( direction == Qsk::LeftToRight || direction == Qsk::TopToBottom );
    animator->setInverted( inverted );

    updateEdge( direction, position );

    setCurrentIndex( position );

    if( position == m_destination )
    {
        Q_EMIT cubeIndexChanged( position );
    }
}

void Cube::switchPosition( const Qsk::Direction direction )
{
    m_destination = neighbor( currentPosition(), direction );

    doSwitch( direction, m_destination );
}

void Cube::switchToPosition( const Position position )
{
    if( currentPosition() == position )
        return;

    m_destination = position;

    const auto direction = this->direction( currentPosition(), position );
    const auto nextPosition = neighbor( currentPosition(), direction );

    doSwitch( direction, nextPosition );
}

Cube::Position Cube::currentPosition() const
{
    return static_cast< Position >( currentIndex() );
}

Cube::Position Cube::neighbor( const Position position, const Qsk::Direction direction ) const
{
    const auto index = s_edgeTransformations[ m_currentEdge ][ direction ];
    const auto n = s_neighbors[ position ][ index ].first;
    return n;
}

Qsk::Direction Cube::direction( const Position from, const Position to ) const
{
    // if direct neighbor: use that direction
    // otherwise: we need 2 swipes, direction doesn't matter, so choose right to left

    const auto neighbors = s_neighbors[ from ];

    for( int i = 0; i < NumEdges; ++i )
    {
        if( neighbors[ i ].first == to )
        {
            return static_cast< Qsk::Direction >( i );
        }
    }

    return Qsk::RightToLeft;
}

void Cube::updateEdge( Qsk::Direction direction, Position position )
{
    m_currentEdge = s_neighbors[ currentPosition() ][ direction ].second;

    // When going back to Front, Left etc., switch back to
    // the bottom edge, otherwise it gets to confusing:
    if( position != TopPos && position != BottomPos )
    {
        m_currentEdge = BottomEdge;
    }
}

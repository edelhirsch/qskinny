/******************************************************************************
 * Copyright (C) 2022 Edelhirsch Software GmbH
 * This file may be used under the terms of the 3-clause BSD License
 *****************************************************************************/

#include "Cube.h"

#include <QskAnimator.h>

#include <array>

QSK_SUBCONTROL( Cube, Panel )

namespace
{
    class CubeAnimator : public QskAnimator
    {
    public:
        CubeAnimator() : QskAnimator()
        {
        }

        void setCube( Cube* cube )
        {
            m_cube = cube;
        }

        virtual void advance( qreal value ) override
        {
            m_cube->setMetric( Cube::Panel | QskAspect::Position, value );
        }

        virtual void done() override
        {
            Q_EMIT m_cube->animationFinished();
        }

    private:
        Cube* m_cube;
    };
}

class Cube::PrivateData
{
  public:
    std::array< QImage, NumPositions > images;
    CubeAnimator animator;
    Position pos;
};

Cube::Cube( QQuickItem* parent )
    : QskControl( parent )
    , m_data( new PrivateData() )
{
    m_data->animator.setCube( this );
    m_data->animator.setDuration( 1250 );
    m_data->animator.setWindow( window() );
}

QImage Cube::image( Position pos ) const
{
    return m_data->images[ pos ];
}

void Cube::setImage( Position pos, const QImage &image )
{
    m_data->images[ pos ] = image;
}

void Cube::startAnimation()
{
    m_data->animator.start();
}

Cube::Position Cube::currentPosition() const
{
    return m_data->pos;
}

void Cube::setCurrentPosition( Position pos )
{
    m_data->pos = pos;
}

#include "moc_Cube.cpp"

/******************************************************************************
 * Copyright (C) 2022 Edelhirsch Software GmbH
 * This file may be used under the terms of the 3-clause BSD License
 *****************************************************************************/

#include "Cube.h"

#include <QskAnimator.h>

#include <array>

QSK_SUBCONTROL( Cube, FrontControl )

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
            m_cube->setMetric( Cube::FrontControl | QskAspect::Position, value );
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
};

Cube::Cube( QQuickItem* parent )
    : QskControl( parent )
    , m_data( new PrivateData() )
{
    m_data->animator.setCube( this );
    m_data->animator.setDuration( 1500 );
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

#include "moc_Cube.cpp"

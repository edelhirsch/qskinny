/******************************************************************************
 * Copyright (C) 2022 Edelhirsch Software GmbH
 * This file may be used under the terms of the 3-clause BSD License
 *****************************************************************************/

#pragma once

#include <QskControl.h>

class Cube : public QskControl
{
    Q_OBJECT

public:
    QSK_SUBCONTROLS( Panel )

    Cube( QQuickItem* parent );

    enum Position
    {
        Front,
        Left,
        Right,
        Top,
        Bottom,
        Back,
        NumPositions
    };

    QImage image( Position pos ) const;
    void setImage( Position pos, const QImage& image );

    Position currentPosition() const;
    void setCurrentPosition( Position pos );

    void startAnimation();

Q_SIGNALS:
    void animationFinished();

private:
    class PrivateData;
    std::unique_ptr< PrivateData > m_data;
};

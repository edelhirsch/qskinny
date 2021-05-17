/******************************************************************************
 * Copyright (C) 2021 Edelhirsch Software GmbH
 * This file may be used under the terms of the 3-clause BSD License
 *****************************************************************************/

#ifndef SKIN_H
#define SKIN_H

#include <QskGradient.h>
#include <QskSkin.h>

class Skin : public QskSkin
{
    public:
        Skin( QObject* parent = nullptr );
        virtual ~Skin();

    private:
        void initHints();
};

#endif // SKIN_H

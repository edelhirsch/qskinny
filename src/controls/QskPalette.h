/******************************************************************************
 * QSkinny - Copyright (C) 2016 Uwe Rathmann
 * This file may be used under the terms of the QSkinny License, Version 1.0
 *****************************************************************************/

#ifndef QSK_PALETTE_H
#define QSK_PALETTE_H

#include "QskAspect.h"
#include "QskGradient.h"

#include <qcolor.h>
#include <qobject.h>

#include <memory>

class QSK_EXPORT QskPalette
{
        Q_GADGET

    public:
        QSK_SUBCONTROLS( Primary, PrimaryVariant, Secondary, SecondaryVariant,
                         Background, Surface, Error, OnPrimary, OnSecondary,
                         OnBackground, OnSurface, OnError )


        QskPalette();
        QskPalette( const QskPalette& other );

        QskPalette( const QskGradient& primary,
                    const QskGradient& primaryVariant,
                    const QskGradient& secondary,
                    const QskGradient& secondaryVariant,
                    const QskGradient& background,
                    const QskGradient& surface,
                    const QskGradient& error,
                    const QskGradient& onPrimary,
                    const QskGradient& onSecondary,
                    const QskGradient& onBackground,
                    const QskGradient& onSurface,
                    const QskGradient& onError );

        virtual ~QskPalette();

        QskGradient primary() const;
        QskGradient primaryVariant() const;
        QskGradient secondary() const;
        QskGradient secondaryVariant() const;
        QskGradient background() const;
        QskGradient surface() const;
        QskGradient error() const;
        QskGradient onPrimary() const;
        QskGradient onSecondary() const;
        QskGradient onBackground() const;
        QskGradient onSurface() const;
        QskGradient onError() const;

    private:
        class PrivateData;
        std::unique_ptr< PrivateData > m_data;
};

#endif

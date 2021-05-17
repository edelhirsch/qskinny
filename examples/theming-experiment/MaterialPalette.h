/******************************************************************************
 * QSkinny - Copyright (C) 2016 Uwe Rathmann
 * This file may be used under the terms of the QSkinny License, Version 1.0
 *****************************************************************************/

#ifndef MATERIAL_PALETTE_H
#define MATERIAL_PALETTE_H

#include <QskPalette.h>

class QSK_EXPORT MaterialPalette : public QskPalette
{
    public:
        MaterialPalette();
        virtual ~MaterialPalette() override;

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

//    private:
//        class PrivateData;
//        std::unique_ptr< PrivateData > m_data;
};

#endif

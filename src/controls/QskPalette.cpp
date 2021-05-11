/******************************************************************************
 * QSkinny - Copyright (C) 2016 Uwe Rathmann
 * This file may be used under the terms of the QSkinny License, Version 1.0
 *****************************************************************************/

#include "QskPalette.h"

QSK_SUBCONTROL( QskPalette, Primary )
QSK_SUBCONTROL( QskPalette, PrimaryVariant )
QSK_SUBCONTROL( QskPalette, Secondary )
QSK_SUBCONTROL( QskPalette, SecondaryVariant )
QSK_SUBCONTROL( QskPalette, Background )
QSK_SUBCONTROL( QskPalette, Surface )
QSK_SUBCONTROL( QskPalette, Error )
QSK_SUBCONTROL( QskPalette, OnPrimary )
QSK_SUBCONTROL( QskPalette, OnSecondary )
QSK_SUBCONTROL( QskPalette, OnBackground )
QSK_SUBCONTROL( QskPalette, OnSurface )
QSK_SUBCONTROL( QskPalette, OnError )


class QskPalette::PrivateData
{
    public:
        QskGradient primary;
        QskGradient primaryVariant;
        QskGradient secondary;
        QskGradient secondaryVariant;
        QskGradient background;
        QskGradient surface;
        QskGradient error;
        QskGradient onPrimary;
        QskGradient onSecondary;
        QskGradient onBackground;
        QskGradient onSurface;
        QskGradient onError;

};

QskPalette::QskPalette()
    : m_data( new PrivateData() )
{
}

QskPalette::QskPalette( const QskPalette& other )
    : m_data( new PrivateData() )
{
    m_data->primary = other.primary();
    m_data->primaryVariant = other.primaryVariant();
    m_data->secondary = other.secondary();
    m_data->secondaryVariant = other.secondaryVariant();
    m_data->background = other.background();
    m_data->surface = other.surface();
    m_data->error = other.error();
    m_data->onPrimary = other.onPrimary();
    m_data->onSecondary = other.onSecondary();
    m_data->onBackground = other.onBackground();
    m_data->onSurface = other.onSurface();
    m_data->onError = other.onError();
}

QskPalette::QskPalette( const QskGradient& primary,
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
                        const QskGradient& onError )
    : m_data( new PrivateData() )
{
    m_data->primary = primary;
    m_data->primaryVariant = primaryVariant;
    m_data->secondary = secondary;
    m_data->secondaryVariant = secondaryVariant;
    m_data->background = background;
    m_data->surface = surface;
    m_data->error = error;
    m_data->onPrimary = onPrimary;
    m_data->onSecondary = onSecondary;
    m_data->onBackground = onBackground;
    m_data->onSurface = onSurface;
    m_data->onError = onError;
}

QskPalette::~QskPalette()
{
}

QskGradient QskPalette::primary() const
{
    return m_data->primary;
}

QskGradient QskPalette::primaryVariant() const
{
    return m_data->primaryVariant;
}

QskGradient QskPalette::secondary() const
{
    return m_data->secondary;
}

QskGradient QskPalette::secondaryVariant() const
{
    return m_data->secondaryVariant;
}

QskGradient QskPalette::background() const
{
    return m_data->background;
}

QskGradient QskPalette::surface() const
{
    return m_data->surface;
}

QskGradient QskPalette::error() const
{
    return m_data->error;
}

QskGradient QskPalette::onPrimary() const
{
    return m_data->onPrimary;
}

QskGradient QskPalette::onSecondary() const
{
    return m_data->onSecondary;
}

QskGradient QskPalette::onBackground() const
{
    return m_data->onBackground;
}

QskGradient QskPalette::onSurface() const
{
    return m_data->onSurface;
}

QskGradient QskPalette::onError() const
{
    return m_data->onError;
}

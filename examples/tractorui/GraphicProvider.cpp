/******************************************************************************
 * Copyright (C) 2021 Edelhirsch Software GmbH
 *           SPDX-License-Identifier: BSD-3-Clause
 *****************************************************************************/

#include "GraphicProvider.h"

#include <QskGraphic.h>
#include <QskGraphicIO.h>

#include <QDebug>
#include <QFile>

const QskGraphic* GraphicProvider::loadGraphic( const QString& id ) const
{
    static QString scope = QStringLiteral( ":/icons/qvg/" );

    const QString name = scope + id + ".qvg";

    QskGraphic graphic = QskGraphicIO::read( name );

    return graphic.isNull() ? nullptr : new QskGraphic( graphic );
}

/******************************************************************************
 * QSkinny - Copyright (C) 2016 Uwe Rathmann
 * This file may be used under the terms of the QSkinny License, Version 1.0
 *****************************************************************************/

#ifndef QSK_HUNSPELL_TEXT_PREDICTOR_H
#define QSK_HUNSPELL_TEXT_PREDICTOR_H

#include <QskTextPredictor.h>
#include <memory>

class QSK_EXPORT QskHunspellTextPredictor : public QskTextPredictor
{
    Q_OBJECT // ### also add to pinyin

    using Inherited = QskTextPredictor;

  public:
    QskHunspellTextPredictor( QObject* = nullptr );
    ~QskHunspellTextPredictor() override;

  protected:
    void request( const QString& ) override;
    void reset() override;

  private:
    class PrivateData;
    std::unique_ptr< PrivateData > m_data;
};

#endif

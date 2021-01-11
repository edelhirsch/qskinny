/******************************************************************************
 * QSkinny - Copyright (C) 2016 Uwe Rathmann
 * This file may be used under the terms of the QSkinny License, Version 1.0
 *****************************************************************************/

#ifndef QSK_TEXT_PREDICTOR_H
#define QSK_TEXT_PREDICTOR_H

#include <QskGlobal.h>
#include <qobject.h>

// abstract base class for input methods for retrieving predictive text

class QSK_EXPORT QskTextPredictor : public QObject
{
    Q_OBJECT

  public:
    enum Attribute
    {
        Words = 1 << 0
    };

    Q_ENUM( Attribute )
    Q_DECLARE_FLAGS( Attributes, Attribute )

    ~QskTextPredictor() override;

    // Doesn't seem to be used yet; once it does, we need to convert it
    // into a slot:
    Attributes attributes() const;

  public Q_SLOTS:
    // ### change Pinyin class as well
    virtual void request( const QString& text ) = 0;
    virtual void reset() = 0;

  Q_SIGNALS:
    // ### maybe we can remove some of the methods above, and cache the candidates
    // from here in the input panel, which runs in the main thread:
    void predictionChanged( const QVector< QString >& candidates );

  protected:
    QskTextPredictor( Attributes, QObject* );

  private:
    const Attributes m_attributes;
};

#endif

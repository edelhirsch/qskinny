#ifndef ALPHASLIDER_H_
#define ALPHASLIDER_H_

#include <QskSlider.h>

class QskAlphaSlider : public QskSlider
{
    Q_OBJECT    
public:
    QSK_SUBCONTROLS(Grid)
    QskAlphaSlider(QQuickItem* parent = nullptr);
};

#endif



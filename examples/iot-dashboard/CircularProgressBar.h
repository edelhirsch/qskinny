#ifndef CIRCULARPROGRESSBAR_H
#define CIRCULARPROGRESSBAR_H

#include <QGradient>
#include <QQuickPaintedItem>

class CircularProgressBar : public QQuickPaintedItem
{
    public:
        CircularProgressBar( const QGradient& gradient, int progress, QQuickItem* parent = nullptr );

        virtual void paint( QPainter* painter ) override;

        double width() const
        {
            return m_width;
        }

        void setWidth( double width )
        {
            m_width = width;
        }

        void setBackgroundColor( const QColor& color )
        {
            m_backgroundColor = color;
        }



    private:
        QGradient m_gradient;
        QColor m_backgroundColor;
        double m_width = 20;
        int m_progress;
};

#endif // CIRCULARPROGRESSBAR_H

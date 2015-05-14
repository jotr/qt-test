#ifndef PLOT_H
#define PLOT_H

#include <QPointF>

class Plot
{
public:
    Plot(QPointF& pos);
    qreal getAge();
    void setAge(qreal age);
    QPointF getPoint();
private:
    qreal m_age;
    QPointF m_pos;
};

#endif // PLOT_H

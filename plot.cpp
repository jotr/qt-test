#include "plot.h"

Plot::Plot(QPointF& pos) : m_age(1.0), m_pos(pos)
{
}

qreal Plot::getAge()
{
    return m_age;
}

void Plot::setAge(qreal age)
{
    m_age = age;
}

QPointF Plot::getPoint()
{
    return m_pos;
}

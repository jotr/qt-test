#include "plotlist.h"
#include <QDebug>

PlotList::PlotList(int size, qreal maxAge) : m_size(size), m_maxAge(maxAge), m_pointsDirty(true)
{
}

void PlotList::addPlot(QPointF& p)
{
    m_plots.enqueue(Plot(p));
    m_points << p;
}

void PlotList::addPlots(QList<Plot> &plots)
{
    for (int i = 0; i < plots.size(); i++)
    {
        m_plots.enqueue(plots[i]);
        m_points.append(plots[i].getPoint());
    }
}

QPolygonF& PlotList::getPoints()
{
    if (m_pointsDirty)
    {
        m_points.clear();
        for (int i = 0; i < m_plots.size(); i++)
        {
            m_points << m_plots[i].getPoint();
        }
        m_pointsDirty = false;
    }
    return m_points;
}


int PlotList::getSize()
{
    return m_size;
}

QRectF PlotList::boundingRect() const
{
    return m_points.boundingRect();
}

void PlotList::stepAge(qreal ageStep, QList<Plot> &removedPlots)
{
    for (int i = 0; i < m_plots.size(); i++)
    {
        qreal age = m_plots[i].getAge();
        m_plots[i].setAge(age - ageStep);
    }

    bool older = true;  //Continue if there might be older plots left
    while (older)
    {
        if (m_plots.isEmpty())
        {
            older = false;
            continue;
        }
        if (m_plots.back().getAge() < m_maxAge)
        {
            Plot p = m_plots.takeLast();
            removedPlots.append(p);
        }
        else
        {
            older = false;
        }
    }
    if (removedPlots.size() > 0)
    {
        m_pointsDirty = true;
    }
}

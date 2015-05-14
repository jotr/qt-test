#ifndef PLOTLIST_H
#define PLOTLIST_H

#include "plot.h"
#include <QQueue>
#include <QPointF>
#include <QPolygonF>

class PlotList
{
public:
    PlotList(int size, qreal maxAge);
    void addPlot(QPointF &p);
    void addPlots(QList<Plot>& plots);
    QPolygonF& getPoints();
    QRectF boundingRect() const;
    int getSize();
    void stepAge(qreal, QList<Plot>& removedPlots);
private:
    int m_size;
    bool m_pointsDirty;
    qreal m_maxAge;
    QQueue<Plot> m_plots;
    QPolygonF m_points; //The current list of the points for drawing

};

#endif // PLOTLIST_H

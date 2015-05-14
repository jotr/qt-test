#include "plotchannelitem.h"
#include "transformutil.h"
#include <QPen>
#include <QPolygonF>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QWidget>
#include <QDebug>



PlotChannelItem::PlotChannelItem() : m_plots1(1, 0.0), m_plots3(3, 0.5), m_plots6(6, 1.0)
{
}


QRectF PlotChannelItem::boundingRect() const
{
    QRectF r1 = m_plots1.boundingRect();
    QRectF r3 = m_plots3.boundingRect();
    QRectF r6 = m_plots6.boundingRect();
    QRectF united = r1.united(r3).united(r6);
    return united;
}

void PlotChannelItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget)
//    const qreal lod = option->levelOfDetailFromTransform(painter->worldTransform());
//    if (size == 3 && lod < 0.1) return;
//    if (size == 1 && lod < 0.25) return;

    //The performance is is gained by using drawPoints()
    //To acheive this, all points are manually transformed and placed in
    //vector for modified points. Then an identity matrix is set up in the painter
    //and the transformed points are drawn using drawPoints()


    drawPoints(painter, m_plots6);
    drawPoints(painter, m_plots3);
    drawPoints(painter, m_plots1);
}

void PlotChannelItem::drawPoints(QPainter *painter, PlotList& plots)
{
    QPolygonF& points = plots.getPoints();
    QPolygonF mappedPoints;

    int size = plots.getSize();
    painter->save();
    QPen pen(Qt::white, size);
    painter->setPen(pen);

//    painter->setTransform(Util::GenerateTranslationOnlyTransform(painter->transform(), points.first()));
    const QTransform& trans = painter->transform();
    for (int i = 0; i < points.size(); ++i) {
        const QPointF& p = points.at(i);
        QTransform t = Util::GenerateTranslationOnlyTransform(trans, p);
        mappedPoints << t.map(p);
//        painter->setTransform(Util::GenerateTranslationOnlyTransform(trans, p));
//        painter->drawPoint(p.x(), p.y());
    }
    painter->setTransform(QTransform());
    painter->drawPoints(mappedPoints);
    painter->restore();
}

void PlotChannelItem::advance(int phase)
{
    if (phase == 0) return;

    if (getRand(10, 1) < 5) return;

    //One plot list for each size: 6, 3, 1
    //Age plots in list
    //Move old plots to next list
    //Plots removed from size 1 list, are recycled into size 6
    QList<Plot> oldPlots;
    m_plots6.stepAge(0.1, oldPlots);
    m_plots3.addPlots(oldPlots);
    //qDebug() << "Plots from 6 -> 3: " << oldPlots.size();
    oldPlots.clear();
    m_plots3.stepAge(0.1, oldPlots);
    m_plots1.addPlots(oldPlots);
    //qDebug() << "Plots from 3 -> 1: " << oldPlots.size();
    oldPlots.clear();
    m_plots1.stepAge(0.1, oldPlots);
    for (int i = 0; i < oldPlots.size(); i++)
    {
        oldPlots[i].setAge(1.0);
    }
    m_plots6.addPlots(oldPlots);
    //qDebug() << "Plots from 1 -> 6: " << oldPlots.size();
}

void PlotChannelItem::addPlot(QPointF p)
{
    m_plots6.addPlot(p);
}

int PlotChannelItem::getRand(int high, int low)
{
    return qrand() % ((high + 1) - low) + low;
}

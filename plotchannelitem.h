#ifndef PLOTCHANNELITEM_H
#define PLOTCHANNELITEM_H

#include <QGraphicsItem>
#include "plotlist.h"

class PlotChannelItem : public QGraphicsItem
{
public:
    PlotChannelItem();
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void advance(int phase);
    void addPlot(QPointF p);
private:
    void drawPoints(QPainter *painter, PlotList& plots);
    int getRand(int high, int low);
    PlotList m_plots1;
    PlotList m_plots3;
    PlotList m_plots6;
    QTransform m_oldTransform;
};

#endif // PLOTCHANNELITEM_H

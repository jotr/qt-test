#ifndef PLOTITEM_H
#define PLOTITEM_H

#include <QGraphicsItem>
#include <QPainter>

class PlotItem : public QGraphicsItem
{
public:
    PlotItem();
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void advance(int phase);

private:
    qreal age;
    int size;
    int type;
};

#endif // PLOTITEM_H

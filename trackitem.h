#ifndef TRACKITEM_H
#define TRACKITEM_H

#include <QGraphicsItem>
#include <QPainter>
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QGraphicsRectItem>
#include <QGraphicsSimpleTextItem>

class TrackItem : public QGraphicsItem
{
public:
    TrackItem();
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void advance(int phase);
private:
    QGraphicsEllipseItem* ellipse;
    QGraphicsLineItem* line;
    QGraphicsRectItem* rect;
    QGraphicsSimpleTextItem* text;
};

#endif // TRACKITEM_H

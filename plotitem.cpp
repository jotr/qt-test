#include "plotitem.h"
#include <QDebug>
#include <QStyleOptionGraphicsItem>
#include "transformutil.h"


int getRand(int high, int low)
{
    return qrand() % ((high + 1) - low) + low;
}


PlotItem::PlotItem() : QGraphicsItem(), age(1.0), size(6)
{
//    setFlag(QGraphicsItem::ItemIgnoresTransformations);
    type = getRand(2, 1);
}

QRectF PlotItem::boundingRect() const
{
    QPointF p = pos();
    return QRectF(p.x() - (size / 2), p.y() - (size / 2), size, size);
}

void PlotItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget)
    const qreal lod = option->levelOfDetailFromTransform(painter->worldTransform());
    if (size == 3 && lod < 0.1) return;
    if (size == 1 && lod < 0.25) return;
    painter->setPen(Qt::NoPen);
//    QPen pen(Qt::white, size);
//    painter->setPen(pen);
//    painter->setPen(QPen(Qt::black));
    painter->setBrush(Qt::white);
    QPointF p = pos();
    //FIXME:
//    QPointF pCenter(p.x(), p.y());
//    QPointF pCenter(p.x(), p.y());
//    painter->drawRect(pCenter.x(), pCenter.y(), size, size);
    painter->save();
    painter->setTransform(Util::GenerateTranslationOnlyTransform(painter->transform(), p));
    // Draw your item.
    painter->drawRect(p.x(), p.y(), size, size);
//    painter->drawPoint(p.x(), p.y());
//    painter->drawText(p.x() + 10, p.y(), QString("Test"));
    painter->restore();
//    qDebug() << "LOD: " << lod;
}

void PlotItem::advance(int phase)
{
    if (phase == 0) return;

    if (getRand(10, 1) < 5) return;

//    setOpacity(age);
    age -= 0.1;
    if (age < 0) {
        age = 1.0;
    }

    int oldSize = size;
    if (age >= 1.0) {
        size = 6;
    } else if (age > 0.5) {
        size = 3;
    } else {
        size = 1;
    }
//    if (oldSize != size) {
//        update();
//    }
}

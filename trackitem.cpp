#include "trackitem.h"
#include "transformutil.h"
#include <QStyleOptionGraphicsItem>
#include <QDebug>
#include <QGraphicsWidget>

TrackItem::TrackItem()
{
    ellipse = new QGraphicsEllipseItem();
    QPen pen(QPen(Qt::white, 0));
    QBrush brush(QColor(255, 255, 255, 32));
    ellipse->setPen(pen);
    ellipse->setBrush(brush);

    line = new QGraphicsLineItem();
    QColor lineColor(Qt::gray);
    pen = QPen(lineColor, 2, Qt::SolidLine, Qt::RoundCap);
    line->setPen(pen);

    rect = new QGraphicsRectItem();
    rect->setBrush(Qt::NoBrush);
    rect->setPen(QPen(Qt::white, 2));

    text = new QGraphicsSimpleTextItem();
    text->setBrush(QBrush(Qt::black));
    QFont font;
    font.setPointSize(11);
    text->setFont(font);
    text->setText(QString("#1234"));
}


QRectF TrackItem::boundingRect() const
{
    QPointF p = pos();
    return QRectF(p.x() - (10 / 2), p.y() - (10 / 2), 10, 10);
}

void TrackItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    //Draw rect
    //Draw line
    //Draw circle
    //Draw text
    //Use LOD
    Q_UNUSED(widget)
    painter->save();
    QPointF p = pos();
    qreal scaleX = painter->transform().m11();
    qreal scaleY = painter->transform().m22();
    const qreal lod = option->levelOfDetailFromTransform(painter->worldTransform());
//    qDebug() << "LOD: " << lod;
    painter->setTransform(Util::GenerateTranslationOnlyTransform(painter->transform(), p));
    QPointF c(p.x(), p.y());

    if (lod > 5) {
        ellipse->setRect(c.x() - 2 / 2 * scaleX, c.y() - 4 / 2 * scaleY, 2 * scaleX, 4 * scaleY);
        ellipse->paint(painter, option);
    }

    if (lod > 0.5) {
        line->setLine(c.x(), c.y(), c.x() + 10 * scaleX, c.y() - 10 * scaleY);
        line->paint(painter, option);
    }
    rect->setRect(p.x() - 6, p.y() - 6, 12, 12);
    rect->paint(painter, option);

    if (lod > 1) {
        painter->setPen(QPen(Qt::black));
//        painter->setBrush(QBrush(Qt::black));
        QFont f(painter->font());
        f.setPointSize(11);
        painter->setFont(f);
        QString str("#12345");
        QFontMetrics metrics(f);
        int width = metrics.width(str);
        int height = metrics.height();
        painter->drawText(c.x() + 10, c.y() + 10, str);
//        text->setPos(c.x() + 100, c.y() + 100);
//        text->paint(painter, option, widget);
    }
    painter->restore();
}


void TrackItem::advance(int phase)
{
    if (phase == 0) return;

    QPointF p = pos();
//    setPos(p.x() + 0.1, p.y() - 0.1);
}

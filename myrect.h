#ifndef MYRECT_H
#define MYRECT_H

#include <QGraphicsRectItem>

class MyRect : public QGraphicsRectItem
{
    Q_OBJECT
public:
    explicit MyRect(qreal x, qreal y, qreal width, qreal height, QGraphicsRectItem *parent = 0);

signals:

public slots:

protected:
    void advance(int phase);

};

#endif // MYRECT_H

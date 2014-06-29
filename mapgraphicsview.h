#ifndef MAPGRAPHICSVIEW_H
#define MAPGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QPolygonF>
#include <QMouseEvent>
#include <QGraphicsItem>
#include <QPropertyAnimation>

class MapGraphicsView : public QGraphicsView
{
    Q_OBJECT
    Q_PROPERTY(qreal translateX MEMBER m_translateX NOTIFY translateChanged)
    Q_PROPERTY(qreal translateY MEMBER m_translateY NOTIFY translateChanged)
    Q_PROPERTY(qreal scale MEMBER m_scale NOTIFY zoomChanged)
public:
    explicit MapGraphicsView(QWidget *parent = 0);
    void addMap(QPolygonF* map);
    void addItem(QGraphicsItem* item);
    void centerItems(QPointF pos);

protected:
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void drawBackground(QPainter* painter, const QRectF & rect);

private:
    QPolygonF* map;
    bool m_panning;
    QPoint panOrigin;
    QMatrix panMatrix;
    QGraphicsItem* m_rootItem;
    bool m_rootAdded;
    QPropertyAnimation *m_animation;
    qreal m_scale;
    qreal m_translateX;
    qreal m_translateY;
    qreal panTranslateX;
    qreal panTranslateY;
    QGraphicsItem* m_vertLine;
    QGraphicsItem* m_horiLine;
    bool m_crossAdded;

    void addCenterCross();

signals:
    void translateChanged();
    void zoomChanged();
public slots:
    void updateState();
    void removeCenterCross();

};

#endif // MAPGRAPHICSVIEW_H

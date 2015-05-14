#include "mapgraphicsview.h"
#include <QDebug>
#include <QLineF>
#include "plotitem.h"


MapGraphicsView::MapGraphicsView(QWidget *parent) :
    QGraphicsView(parent),
    m_panning(false), m_rootAdded(false), m_crossAdded(false),
    m_scale(1.0), m_translateX(0.0), m_translateY(0.0)
{
    QGraphicsRectItem* item = new QGraphicsRectItem(0, 0, 10, 10);
    item->setPen(Qt::NoPen);
//    item->setPen(QPen(Qt::white));
//    item->pen().setWidth(0);
    m_rootItem = item;
    m_animation = new QPropertyAnimation(this);
    connect(this, SIGNAL(translateChanged()), this, SLOT(updateState()));
    connect(this, SIGNAL(zoomChanged()), this, SLOT(updateState()));
}

void MapGraphicsView::addItem(QGraphicsItem *item)
{
    if (!m_rootAdded) {
        scene()->addItem(m_rootItem);
        m_rootAdded = true;
    }
    item->setParentItem(m_rootItem); //Setting parent automatically adds it to parents scene
}

void MapGraphicsView::addMap(QPolygonF *map)
{
    this->map = map;
}

void MapGraphicsView::centerItems(QPointF pos)
{
    m_translateX = pos.x();
    m_translateY = pos.y();
    updateState();
}

void MapGraphicsView::mousePressEvent(QMouseEvent *event)
{
//    QGraphicsView::mousePressEvent(event);
    if (event->button() == Qt::LeftButton && event->modifiers().testFlag(Qt::ControlModifier)) {
        QMatrix mInv = m_rootItem->matrix().inverted();
        QPointF ep = mapToScene(event->pos()) * mInv;

//        QGraphicsRectItem* item = new QGraphicsRectItem(ep.x(), ep.y(), 10, 10);
        QString str = QString("%1:%2").arg(ep.x()).arg(ep.y());
        QGraphicsTextItem* item = new QGraphicsTextItem(str);
        QFont localFont = item->font();
        localFont.setPointSize(11);
        item->setFont(localFont);
//        item->setDefaultTextColor(Qt::gray);
        item->setFlag(QGraphicsItem::ItemIgnoresTransformations);
//        PlotItem* item = new PlotItem();
//        item->setPen(QPen(Qt::white));
        item->setPos(ep);
        addItem(item);
        event->accept();
    } else if (event->button() == Qt::RightButton){
        m_panning = true;
        panOrigin = event->pos();
        panTranslateX = m_translateX;
        panTranslateY = m_translateY;
        viewport()->setCursor(Qt::ClosedHandCursor);
        addCenterCross();
        event->accept();
    }
}

void MapGraphicsView::addCenterCross()
{
    if (m_crossAdded) {
        return;
    }
    QLineF lv(0, -10, 0, 10);
    QLineF lh(-10, 0, 10, 0);
    m_vertLine = scene()->addLine(lv, QPen(Qt::white));
    m_horiLine = scene()->addLine(lh, QPen(Qt::white));
    m_crossAdded = true;
}

void MapGraphicsView::removeCenterCross()
{
    if (!m_crossAdded) {
        return;
    }
    scene()->removeItem(m_vertLine);
    scene()->removeItem(m_horiLine);
    delete m_vertLine;
    m_vertLine = 0;
    delete m_horiLine;
    m_horiLine = 0;
    m_crossAdded = false;
}

void MapGraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
//    QGraphicsView::mouseReleaseEvent(event);
    if (m_panning) {
        m_panning = false;
        viewport()->setCursor(Qt::ArrowCursor);
        removeCenterCross();
        updateState();
        event->accept();
    }
}

void MapGraphicsView::mouseMoveEvent(QMouseEvent* event)
{
//    QGraphicsView::mouseMoveEvent(event);

    if (m_panning) {
        if (m_animation->state() == QPropertyAnimation::Running) {
            m_animation->stop();
        }
        if (!m_crossAdded) {  //Happens sometimes when panning starts before zooming is done
            addCenterCross();
        }

        QPoint p = event->pos();
        QPoint pO = panOrigin;
        qreal offsetX = p.x() - pO.x();
        qreal offsetY = p.y() - pO.y();
        //Calculate the translate from the base of
        m_translateX = panTranslateX + offsetX / m_scale;
        m_translateY = panTranslateY + offsetY / m_scale;
        updateState();
        event->accept();
    }
}

void MapGraphicsView::wheelEvent(QWheelEvent *event)
{
    qreal oldScale = m_scale;
    qreal newScale;
    double scaleFactor = 1.8;

    // Scale the view / do the zoom
    if(event->delta() > 0) {
        // Zooming in
        newScale = oldScale * scaleFactor;
    } else if(event->delta() < 0) {
        // Zooming out
        newScale = oldScale * 1.0 / scaleFactor;
    }

    //The idea is to scale the position under the mouse
    //using the new scale and then translate using the difference
    //between the new pos and the old
//    QMatrix mInv = m_rootItem->matrix().inverted();
//    QPointF ep = mapToScene(event->pos()) * mInv;

//    qreal scaledX = ep.x() / newScale;
//    qreal scaledY = ep.y() / newScale;

//    qreal diffX = scaledX - ep.x();
//    qreal diffY = scaledY - ep.y();

////    qDebug() << "diffX: " << diffX << " diffY: " << diffY;

//    m_translateX += diffX / m_scale;
//    m_translateY += diffY / m_scale;

//    m_scale = newScale;
//    updateState();

    //Animation below
    addCenterCross();
    m_animation->stop();
    m_animation->setTargetObject(this);
    m_animation->setPropertyName("scale");
    m_animation->setDuration(500);
    m_animation->setStartValue(oldScale);
    m_animation->setEndValue(newScale);
    m_animation->setEasingCurve(QEasingCurve::OutCubic);
    m_animation->start();
    connect(m_animation, SIGNAL(finished()), this, SLOT(removeCenterCross()));
    event->accept();
}

void MapGraphicsView::keyPressEvent(QKeyEvent *event)
{
    QByteArray propertyName;
    qreal startValue, endValue;
    qreal scaleFactor = 1.5;

    if (event->key() == Qt::Key_Left) {
        propertyName = "translateX";
        startValue = m_translateX;
        endValue = m_translateX + 100 / m_scale;
    } else if (event->key() == Qt::Key_Right) {
        propertyName = "translateX";
        startValue = m_translateX;
        endValue = m_translateX - 100 / m_scale;
    } else if (event->key() == Qt::Key_Up) {
        propertyName = "translateY";
        startValue = m_translateY;
        endValue = m_translateY + 100 / m_scale;
    } else if (event->key() == Qt::Key_Down) {
        propertyName = "translateY";
        startValue = m_translateY;
        endValue = m_translateY - 100 / m_scale;
    } else if (event->key() == Qt::Key_Plus) {
        propertyName = "scale";
        startValue = m_scale;
        endValue = m_scale * scaleFactor;
    } else if (event->key() == Qt::Key_Minus) {
        propertyName = "scale";
        startValue = m_scale;
        endValue = m_scale * 1.0 / scaleFactor;
    } else {
        return;
    }
    addCenterCross();
    m_animation->stop();
    m_animation->setTargetObject(this);
    m_animation->setPropertyName(propertyName);
    m_animation->setDuration(500);
    m_animation->setStartValue(startValue);
    m_animation->setEndValue(endValue);
    m_animation->setEasingCurve(QEasingCurve::OutCubic);
    m_animation->start();
    connect(m_animation, SIGNAL(finished()), this, SLOT(removeCenterCross()));
    event->accept();
}

void MapGraphicsView::updateState()
{
    QMatrix m;
    m.scale(m_scale, m_scale);
    m.translate(m_translateX, m_translateY);
    m_rootItem->setMatrix(m);
    resetCachedContent();
}

void MapGraphicsView::drawBackground(QPainter *painter, const QRectF &rect)
{
    QColor c(Qt::darkGray);
    QBrush b(c.darker());
    painter->setBrush(b);
    painter->drawRect(rect);
    if (map != 0) {
        QPen pen(Qt::green);
        pen.setWidth(0);
        painter->setPen(pen);
        QBrush brush(Qt::darkGreen);
        painter->setBrush(brush);
        QMatrix m;
        m.scale(m_scale, m_scale);
        m.translate(m_translateX, m_translateY);
        painter->setMatrix(m, true);
        painter->setRenderHint(QPainter::Antialiasing);
        painter->drawPolygon(*map);
        //TODO: Use a polygon item and set setTransformationMode(Qt::SmoothTransformation)
    }
}

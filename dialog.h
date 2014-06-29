#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QTimer>
#include "mapgraphicsview.h"

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();
    void setMap(QPolygonF* poly);
//    bool eventFilter(QObject *, QEvent *);

private slots:

private:
    Ui::Dialog *ui;
    QGraphicsScene *scene;
    MapGraphicsView* myView;
//    QGraphicsRectItem *rect1;
//    QGraphicsRectItem *rect2;
    qreal scale;
    QGraphicsPolygonItem* polyItem;
    QTimer *timer;
};

#endif // DIALOG_H

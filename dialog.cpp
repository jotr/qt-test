#include "dialog.h"
#include "ui_dialog.h"
#include "shapelib/shapefil.h"
#include <QDebug>
#include "plotitem.h"
#include "trackitem.h"
#include <QtOpenGL>
#include <QWheelEvent>


void loadMapData(std::string path, QPolygonF* poly)
{
    SHPHandle shp = SHPOpen(path.c_str(), "rb");
    if (shp != 0) {
        qDebug() << "Loaded map: " << path.c_str();
    } else {
        qDebug() << "Could not load map: " << path.c_str();
        return;
    }
    int entities, type;
    double minBound[4], maxBound[4];
    SHPGetInfo(shp, &entities, &type, minBound, maxBound);
//    qDebug() << "Type: " << type << " Entities: " << entities;

    SHPObject* o = SHPReadObject(shp, 0);

    int parts = o->nParts;

//    qDebug() << "Parts: " << parts;

    int currentPart = 0;
    int start = o->panPartStart[currentPart];
    int end = parts == 1 ? o->nVertices : o->panPartStart[currentPart + 1];

//    qDebug() << "Start: " << start << " End: " << end << " Vertices: " << o->nVertices;

    for(int i = start; i < end; i++) {
        double x = o->padfX[i];
        double y = o->padfY[i];
        *poly << QPointF(x, y);
    }
}



Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog),
    scale(1.0)
{
    ui->setupUi(this);


    myView = new MapGraphicsView(this);

    QVBoxLayout* layout = new QVBoxLayout();
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(myView);

    ui->frame->setLayout(layout);

//    myView->viewport()->installEventFilter(this);

//    myView->setViewport(new QGLWidget());  //Great boost. Poor rendering for zero width lines

    scene = new QGraphicsScene(this);
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);   //Disables BSP -> Great boost!
//    scene->setItemIndexMethod(QGraphicsScene::BspTreeIndex);
    myView->setScene(scene);
    myView->setHorizontalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
    myView->setVerticalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
//    myView->setDragMode(QGraphicsView::ScrollHandDrag);
//    myView->setDragMode(QGraphicsView::NoDrag);
//    myView->setDragMode(QGraphicsView::RubberBandDrag);

    myView->setOptimizationFlags(QGraphicsView::DontSavePainterState);  //Minor boost
//    myView->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    myView->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);   //Minor boost
    myView->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
//    myView->setRenderHint(QPainter::Antialiasing, false);


    QColor c(Qt::darkGray);
    QColor cd = c.darker();
    QColor bg(127, 125, 158);

//    scene->setBackgroundBrush(QBrush(cd));
    myView->setCacheMode(QGraphicsView::CacheBackground);

    int xPos = 0;
    int yPos = 0;

    int row = 100;
    int col = 100;

    QPolygonF* poly = new QPolygonF();
    loadMapData(std::string("/Users/johan/Programmering/Qt/ViewTest/ALG_outline_SHP/ALG_outline"), poly);
    setMap(poly);

//    QMatrix m;
//    m.scale(1, -1);

    for(int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            PlotItem* plot = new PlotItem();  //Not actually leaking. The scene will own it
            plot->setPos(xPos, yPos);
            myView->addItem(plot);
            xPos += 15;
        }
        yPos += 15;
        xPos = 0;
    }

    TrackItem* track = new TrackItem();
    track->setPos(2, -15);
    myView->addItem(track);

//    int maxX = row * 15;
//    int maxY = col * 15;

//    ui->graphicsView->setSceneRect(-maxX, -maxY, maxX, maxY);

//    myView->setSceneRect(-100, -100, 100, 100);

    qDebug() << "Item count: " << row * col;

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()),scene, SLOT(advance()));
    connect(timer, SIGNAL(timeout()), scene, SLOT(update()));
    timer->start(1000);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::setMap(QPolygonF *poly)
{
    QPen pen(Qt::green);
    pen.setWidth(0);
    QGraphicsPolygonItem* item = new QGraphicsPolygonItem(*poly);
    item->setPen(pen);
    item->setBrush(QBrush(Qt::darkGreen)); //, Qt::BDiagPattern)
    item->setZValue(-1);
    QMatrix m;
    m.scale(1, -1);
//    m.translate(60, -90);
    item->setMatrix(m);
//    QRectF bounds = item->sceneBoundingRect();
    QRectF bounds = item->boundingRect();
//    qDebug() << "Bounds: " << bounds;
//    qDebug() << "Bounds center: " << bounds.center();
    qreal viewHeight = myView->viewport()->rect().height();
    qreal sceneHeight = bounds.height();
    qreal ratio = 460 / sceneHeight;  //FIXME: Get actual height instead
//    qDebug() << "View/Scene ratio: " << ratio << " hv:" << viewHeight << " hs: " << sceneHeight;
    myView->setProperty("scale", ratio / 1.5);
    myView->centerItems(bounds.center());
    myView->setSceneRect(QRect(0, 0, 1, 1));  //To synchronize model and view center
    myView->addMap(poly);
//    myView->addItem(item);
//    polyItem->setCacheMode(QGraphicsItem::DeviceCoordinateCache);  //Boost?
}

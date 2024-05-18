#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <vector>
#include <QPolygonF>
#include <QPointF>
#include <QInputDialog>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QGraphicsTextItem>
#include <QFile>
#include <QXmlStreamWriter>
#include <QMessageBox>
#include <QFileDialog>



class Obstacle {
public:
    std::vector<int> indexes;
    std::vector<QPolygonF> Polygons;
    QPolygonF Polygon;

    void addPointToPolygon(const QPointF &point);
    void finalizePolygon(QGraphicsScene *scene);
    void clearAll();
    void saveToXml();
};



#endif // OBSTACLE_H

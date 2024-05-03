#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <QGraphicsScene>
#include <QGraphicsPolygonItem>
#include <QGraphicsSimpleTextItem>
#include <QFont>
#include <QPolygonF>

/**
 * @brief The Obstacle class
 */
class Obstacle{
public:
    Obstacle(QGraphicsScene* scene, const QPolygonF& polygon, int impasseIndex);
    ~Obstacle();

    void create();


private:
    QGraphicsScene* scene;
    QPolygonF polygon;
    int impasseIndex;
};

#endif // OBSTACLE_H

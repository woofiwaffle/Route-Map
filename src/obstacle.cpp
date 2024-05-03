#include "headers/obstacle.h"



Obstacle::Obstacle(QGraphicsScene* scene, const QPolygonF& polygon, int impasseIndex)
    : scene(scene), polygon(polygon), impasseIndex(impasseIndex) {}



Obstacle::~Obstacle() {}



void Obstacle::create() {
    QGraphicsPolygonItem* obstacleItem = scene->addPolygon(polygon);
    obstacleItem->setBrush(Qt::white);

    QString indexText = QString::number(impasseIndex) + "%";
    QGraphicsSimpleTextItem* indexTextItem = scene->addSimpleText(indexText);
    indexTextItem->setFont(QFont("Arial", 10));
    indexTextItem->setPos(polygon.boundingRect().center() - QPointF(10, 10));
}

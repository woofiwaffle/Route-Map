#include "headers/moveitem.h"

MoveItem::MoveItem(QObject *parent) : QObject(parent), QGraphicsItem() {}

MoveItem::~MoveItem() {}



QRectF MoveItem::boundingRect() const {
    return QRectF (-30,-30,60,60);
}



void MoveItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    painter->setPen(Qt::green);
    painter->setBrush(Qt::green);
    painter->drawPoint(0,0);
    Q_UNUSED(option);
    Q_UNUSED(widget);
}



void MoveItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
     /*Устанавливаем позицию графического элемента
     * в графической сцене, транслировав координаты
     * курсора внутри графического элемента
     * в координатную систему графической сцены
     *
    this->setPos(mapToScene(event->pos()));
    emit pointAdded();
    Q_UNUSED(event);*/
}



void MoveItem::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    /* При нажатии мышью на графический элемент
     * заменяем курсор на руку, которая держит этот элемента
     *
    this->setCursor(QCursor(Qt::ClosedHandCursor));
    Q_UNUSED(event);*/
}



void MoveItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    /* При отпускании мышью элемента
     * заменяем на обычный курсор стрелку
     *
     *
    this->setCursor(QCursor(Qt::ArrowCursor));

    Q_UNUSED(event);*/
}






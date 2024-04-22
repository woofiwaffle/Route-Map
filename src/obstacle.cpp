#include "headers/obstacle.h"


TObstacle::TObstacle() : m_impasseIndex(0) {}

TObstacle::TObstacle(const QPointF& pos, int impasseIndex) : m_pos(pos), m_impasseIndex(impasseIndex) {}

//Obstacle::~Obstacle() {}




QPointF TObstacle::getPos() const{
    return m_pos; // получение позиции препятствия
}



int TObstacle::getImpasseIndex() const{
    return m_impasseIndex; // получить индекс тупика
}

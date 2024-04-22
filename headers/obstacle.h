#ifndef TOBSTACLE_H
#define TOBSTACLE_H

#include <QPolygonF>

/**
 * @brief The Obstacle class
 */
class TObstacle{
public:
    TObstacle();
    //~Obstacle();
    TObstacle(const QPointF& pos, int impasseIndex);

    QPointF getPos() const;
    int getImpasseIndex() const;


private:
    QPointF m_pos;
    int m_impasseIndex;
};

#endif // TOBSTACLE_H

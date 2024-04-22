#ifndef TROUTE_H
#define TROUTE_H


#include <QPolygonF>

/**
 * @brief The Route class
 */
class TRoute{
public:
    TRoute(const QPolygonF& points, qreal length, qreal time);              // методы для вычисления маршрута

    QPolygonF getPoints() const;
    qreal getLength() const;
    qreal getTime() const;


private:
    QPolygonF m_points;
    qreal m_length;
    qreal m_time;
};

#endif // TROUTE_H

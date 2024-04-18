#include "route.h"


TRoute::TRoute(const QPolygonF& points, qreal length, qreal time) : m_points(points), m_length(length), m_time(time) {}



QPolygonF TRoute::getPoints() const{  // получение точек
    return m_points;
}




qreal TRoute::getLength() const{  // получение длины
    return m_length;
}



qreal TRoute::getTime() const{   // получение времени
    return m_time;
}

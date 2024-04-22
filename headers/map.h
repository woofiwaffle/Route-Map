#ifndef TMAP_H
#define TMAP_H

#include "obstacle.h"
#include "route.h"

#include <QObject>
#include <QVector>

/**
 * @brief The Map class
 */
class TMap : public QObject{
    Q_OBJECT

public:

    TMap(QObject *parent = nullptr);
    ~TMap();

    TRoute findOptimalRoute() const;
    void addObstacle(const TObstacle& obstacle);

private:
    QVector<TObstacle> obstacles;
};

#endif // TMAP_H

#ifndef ROUTE_H
#define ROUTE_H

#include <QGraphicsItem>
#include <QObject>
#include <QGraphicsScene>
#include <QXmlStreamReader>
#include <QFile>
#include <vector>

using namespace std;


class Route : public QObject {
    Q_OBJECT

public:
    explicit Route(QObject *parent = nullptr);              // методы для вычисления маршрута

protected:
    //void loadMapFromXml(const QString& fileName);
    //void setStartPoint(QGraphicsItem startPoint);
    //void setFinishPoint(QGraphicsItem finishPoint);
    //void findOptimalRoute();
    //void saveRouteToFile(const QString& fileName);

    //vector<vector<QGraphicsItem*>> Polygons;
    //vector<int> indexes;

private:
    QGraphicsScene* scene;
//    vector<QGraphicsItem*> m_obstacles;
//    QGraphicsItem* m_startPoint;
//    QGraphicsItem* m_finishPoint;
//    vector<QGraphicsItem*> m_optimalRoute;

//    vector<vector<QGraphicsItem*>> Polygons;
  //  vector<int> indexes;

//    void clearRoute();
};


#endif // TROUTE_H

//#ifndef ROUTE_H
//#define ROUTE_H

//#include <QMessageBox>
//#include <QGraphicsScene>
//#include <QFile>
//#include <QXmlStreamWriter>
//#include <QPolygonF>
//#include <vector>
//#include <queue>
//#include <cmath>
//#include <unordered_map>
//#include <QGraphicsScene>
//#include <QGraphicsTextItem>
//#include <qDebug>
//#include <QFileDialog>


//struct Node {
//    QPointF Point;
//    double cost;
//    int heuristic;

//    Node(int x, int y, double cost, int heuristic)
//        : Point(x, y), cost(cost), heuristic(heuristic) {}

//    Node() : Point(0,0), cost(0), heuristic(0) {}

//    bool operator>(const Node& other) const {
//        // Сравниваем сумму стоимости и эвристической оценки
//        return (cost + heuristic) > (other.cost + other.heuristic);
//    }
//};



//class Route {
//public:
//    int n = 1200;
//    int flag = 1;
//    QPointF* StartPoint = nullptr;
//    QPointF* FinishPoint = nullptr;

//    std::vector<int> indexes;
//    std::vector<QPolygonF> Polygons;
//    std::vector<Node> WayPoints;
//    QPolygonF Polygon;

//    void loadMapFromXml(const QString& fileName, QGraphicsScene* scene);
//    void clearWay();
//    void startJourney();
//    void saveRoute();

//private:
//    void findOptimalRoute(QPointF* start, QPointF* finish);
//    double distance(QPointF* current, QPointF* neighbor);
//    std::vector<QPointF*> reconstructPath(Node* endNode);


//    int findCost(Node*, Node*);
//    std::vector<Node> getNeighbors(Node*, Node*, int);
//    bool searchPoint(QPointF);
//    std::vector<Node> aStar(Node, Node, int);
//};

//#endif // ROUTE_H

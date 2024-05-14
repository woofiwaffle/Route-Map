#ifndef INTERFACEROUTE_H
#define INTERFACEROUTE_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QFileDialog>
#include <QInputDialog>
#include <QGraphicsItem>
#include <QXmlStreamReader>
#include <QDebug>
#include <queue>
#include <cmath>
#include <unordered_map>


using namespace std;



struct Node {
    QPointF Point;
    int cost;
    int heuristic;

    Node(int x, int y, int cost, int heuristic)
        : Point(x, y), cost(cost), heuristic(heuristic) {}
};


namespace Ui { class InterfaceRoute; }

class InterfaceRoute : public QWidget {
    Q_OBJECT

public:
    InterfaceRoute(QWidget *parent = nullptr);
    ~InterfaceRoute();

private:
    Ui::InterfaceRoute *ui;
    QGraphicsScene *scene;

    int n = 600;
    int flag = 1;
    QPointF* StartPoint = nullptr;
    QPointF* FinishPoint = nullptr;

    std::vector<int> indexes;
    vector <QPolygonF> Polygons;
    QPolygonF Polygon;

    void loadMapFromXml(const QString& fileName);
    void findOptimalRoute(QPointF* start, QPointF* finish);
    //double heuristic(QPointF* current, QPointF* finish);
    //vector<QPointF*> getNeighbors(QPointF* current);
    double distance(QPointF* current, QPointF* neighbor);
    vector<QPointF*> reconstructPath(Node* endNode);


    int findCost(Node*, Node*, Node*);
    std::vector<Node> getNeighbors(Node*, Node*, Node*, int);
    Node MinCost(vector<Node>);
    bool searchPoint(QPointF);
    std::vector<Node> aStar(Node, Node, int);


private slots:
    void mousePressEvent(QMouseEvent *);
    void backToMain();
    void on_button_LoadingMap_clicked();
    void on_button_StartJourney_clicked();
    //void on_button_Save_clicked();
};









#endif // INTERFACEROUTE_H

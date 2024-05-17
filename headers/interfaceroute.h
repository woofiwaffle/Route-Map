#ifndef INTERFACEROUTE_H
#define INTERFACEROUTE_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QFileDialog>
#include <QInputDialog>
#include <QGraphicsItem>
#include <QXmlStreamReader>
#include <QMessageBox>
#include <QDebug>
#include <queue>
#include <cmath>
#include <unordered_map>


using namespace std;



struct Node {
    QPointF Point;
    double cost;
    int heuristic;

    Node(int x, int y, double cost, int heuristic)
        : Point(x, y), cost(cost), heuristic(heuristic) {}

    Node() : Point(0,0), cost(0), heuristic(0) {}

    bool operator>(const Node& other) const {
        // Сравниваем сумму стоимости и эвристической оценки
        return (cost + heuristic) > (other.cost + other.heuristic);
    }
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

    int n = 1200;
    int flag = 1;
    QPointF* StartPoint = nullptr;
    QPointF* FinishPoint = nullptr;

    std::vector<int> indexes;
    vector <QPolygonF> Polygons;
    vector<Node> WayPoints;
    QPolygonF Polygon;

    void loadMapFromXml(const QString& fileName);
    void findOptimalRoute(QPointF* start, QPointF* finish);
    double distance(QPointF* current, QPointF* neighbor);
    vector<QPointF*> reconstructPath(Node* endNode);


    int findCost(Node*, Node*);
    std::vector<Node> getNeighbors(Node*, Node*, int);
    bool searchPoint(QPointF);
    std::vector<Node> aStar(Node, Node, int);

private slots:
    void mousePressEvent(QMouseEvent *);
    void backToMain();
    void on_button_LoadingMap_clicked();
    void on_button_StartJourney_clicked();
    void on_button_Save_clicked();
    void on_button_ClearWay_clicked();
};


#endif // INTERFACEROUTE_H

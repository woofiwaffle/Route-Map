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


using namespace std;


namespace Ui { class InterfaceRoute; }



class InterfaceRoute : public QWidget {
    Q_OBJECT

public:
    InterfaceRoute(QWidget *parent = nullptr);
    ~InterfaceRoute();

private:
    Ui::InterfaceRoute *ui;
    QGraphicsScene *scene;

    QGraphicsItem* StartPoint;
    QGraphicsItem* FinishPoint;

    std::vector<int> indexes;
    vector <QPolygonF> Polygons;
    QPolygonF Polygon;

    void loadMapFromXml(const QString& fileName);
    void findOptimalRoute(QGraphicsItem* start, QGraphicsItem* finish);

private slots:
    double heuristic(QPointF*, QPointF*);
    void backToMain();
    void on_button_LoadingMap_clicked();
    void on_button_StartJourney_clicked();
    //void on_button_Save_clicked();
};

#endif // INTERFACEROUTE_H

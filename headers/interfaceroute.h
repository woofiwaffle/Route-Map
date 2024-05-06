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

#include "headers/moveitem.h"


namespace Ui { class InterfaceRoute; }



class InterfaceRoute : public QWidget {
    Q_OBJECT

public:
    InterfaceRoute(QWidget *parent = nullptr);
    ~InterfaceRoute();

   void mousePressEvent(QMouseEvent *event);

private:
    Ui::InterfaceRoute *ui;
    QGraphicsScene *scene;

    QGraphicsItem* StartPoint;
    QGraphicsItem* FinishPoint;


    std::vector<std::vector<QGraphicsItem*>> Polygons;
    std::vector<int> indexes;

    void loadMapFromXml(const QString& fileName);
    void findOptimalRoute(QGraphicsItem* start, QGraphicsItem* finish);

private slots:
    void backToMain();
    void on_button_LoadingMap_clicked();
    void on_button_StartJourney_clicked();
    //void on_button_Save_clicked();
};

#endif // INTERFACEROUTE_H

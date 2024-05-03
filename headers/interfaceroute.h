#ifndef INTERFACEROUTE_H
#define INTERFACEROUTE_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>

#include "headers/moveitem.h"
#include "headers/interfacemap.h"

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


    vector<vector<QGraphicsItem*>> Polygons;
    vector<int> indexes;

    void loadMapFromXml(const QString& fileName);

private slots:
    void backToMain();
    void on_button_LoadingMap_clicked();
    void on_button_StartJourney_clicked();
    void on_button_FinishJourney_clicked();
    //void on_button_Save_clicked();
};

#endif // INTERFACEROUTE_H

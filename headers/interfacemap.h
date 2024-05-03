#ifndef INTERFACEMAP_H
#define INTERFACEMAP_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
//#include <QGraphicsLineItem>
#include <QMouseEvent>
//#include <QEvent>
#include <QFileDialog>
#include <QInputDialog>
#include <QGraphicsItem>
//#include <QDebug>
#include <QXmlStreamWriter>
#include <vector>

#include <headers/moveitem.h>


using namespace std;

enum CustomRoles {
    passIndexRole = Qt::UserRole + 1
};


namespace Ui { class InterfaceMap; }



class InterfaceMap : public QWidget {
    Q_OBJECT

public:
    explicit InterfaceMap(QWidget *parent = nullptr);
    ~InterfaceMap();

    //void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    //void mouseReleaseEvent(QMouseEvent *event);

private:
    Ui::InterfaceMap *ui;
    QGraphicsScene *scene;


    vector<vector<QGraphicsItem*>> Polygons;
    vector<int> indexes;

    vector<QGraphicsItem*> Points;

    bool search(QGraphicsItem*);
    void createLine();
    void updateLine();

protected:

private slots:
    void backToMain();
    void on_button_CreateObstacle_clicked();
    void on_button_ClearMap_clicked();
    void on_button_Save_clicked();
};

#endif // INTERFACEMAP_H

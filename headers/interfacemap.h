#ifndef INTERFACEMAP_H
#define INTERFACEMAP_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMouseEvent>
#include <QFileDialog>
#include <QInputDialog>
#include <QGraphicsItem>
#include <QXmlStreamWriter>
#include <QDebug>
#include <vector>

#include <headers/moveitem.h>


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


    std::vector<std::vector<QGraphicsItem*>> Polygons;
    std::vector<int> indexes;

    std::vector<QGraphicsItem*> Points;

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

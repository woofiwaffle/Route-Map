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

    void mousePressEvent(QMouseEvent *event);

private:
    Ui::InterfaceMap *ui;
    QGraphicsScene *scene;


    std::vector<int> indexes;
    vector <QPolygonF> Polygons;
    QPolygonF Polygon;

protected:

private slots:
    void backToMain();
    void on_button_ClearMap_clicked();
    void on_button_Save_clicked();
};

#endif // INTERFACEMAP_H

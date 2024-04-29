#ifndef INTERFACE_H
#define INTERFACE_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsLineItem>
#include <QMouseEvent>
#include <QEvent>
#include <QDomDocument>
#include <QFileDialog>
#include <QGraphicsItem>
#include <QDebug>
#include <vector>
#include <headers/moveitem.h>


using namespace std;

namespace Ui {
class TInterface;
}

class TInterface : public QWidget {
    Q_OBJECT

public:
    explicit TInterface(QWidget *parent = nullptr);
    ~TInterface();

    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    Ui::TInterface *ui;
    QGraphicsScene *scene;
    vector<vector<QGraphicsItem*>> Polygons; // фигуры с адресами точек
    vector<QGraphicsItem*> Points; // точки использованных фигур

    bool poisk(QGraphicsItem*);
    void createLine();
    void updateLine();

protected:

private slots:
    void backToMain();
    void on_button_CreateLine_clicked();
    void on_button_ClearMap_clicked();
    void on_button_Save_clicked();
};

#endif // INTERFACE_H

#ifndef INTERFACE_H
#define INTERFACE_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QMouseEvent>
#include <QEvent>

#include <QObject>
#include <QGraphicsItem>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <QCursor>
#include <vector>

#include <random>
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
    vector<QGraphicsItem*> Points;

    bool poisk(QGraphicsItem*);
    void createLine();
    void updateLine();

protected:

private slots:
    void backToMain();
    void on_button_NewPoint_clicked();
    //void on_button_NewLine_clicked();
    //void on_button_Index_clicked();
    //void on_button_Save_clicked();
};

#endif // INTERFACE_H

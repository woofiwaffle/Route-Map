#ifndef INTERFACE_H
#define INTERFACE_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QMouseEvent>
#include <QEvent>


#include <src/moveitem.h>

namespace Ui {
class TInterface;
}

class TInterface : public QWidget {
    Q_OBJECT

public:
    explicit TInterface(QWidget *parent = nullptr);
    ~TInterface();

private:
    Ui::TInterface *ui;
    QWidget *editScreen;
    QWidget *mapScreen;
    QGraphicsScene *scene;

protected:
    //virtual void MousePressEvent(QMouseEvent*);

private slots:
    void backToMain();
    void on_button_Back_4_clicked();
};

#endif // INTERFACE_H

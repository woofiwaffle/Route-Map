#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>

#include "headers/interfaceroute.h"
#include "headers/interfacemap.h"

namespace Ui { class MainWindow; }


class MainWindow : public QWidget {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

private slots:
    void openMapWindow();
    void openRouteWindow();
};

#endif // MAINWINDOW_H

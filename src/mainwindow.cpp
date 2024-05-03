#include "headers/mainwindow.h"
#include "headers/interfaceroute.h"
#include "headers/interfacemap.h"

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent): QWidget(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    connect(ui->button_Route, &QPushButton::clicked, this, &MainWindow::openRouteWindow);
    connect(ui->button_Map, &QPushButton::clicked, this, &MainWindow::openMapWindow);
}



MainWindow::~MainWindow() {
    delete ui;
}


void MainWindow::openRouteWindow() {
    InterfaceRoute *routeWindow = new InterfaceRoute();
    this->hide();
    routeWindow->show();
}



void MainWindow::openMapWindow() {
    InterfaceMap *mapWindow = new InterfaceMap();
    this->hide();
    mapWindow->show();
}

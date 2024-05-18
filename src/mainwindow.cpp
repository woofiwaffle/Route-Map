#include "headers/mainwindow.h"

#include "ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent): QWidget(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    QString buttonStyle = "QPushButton {"
                              " background-color: #333333;"
                              " color: #FFFFFF;"
                              " border: 1px solid #FFFFFF;"
                              " border-radius: 5px;"
                              " padding: 10px;"
                              "}"
                              "QPushButton:hover {"
                              " background-color: #444444;"
                              "}"
                              "QPushButton:pressed {"
                              " background-color: #666666;"
                              "}";
    ui->button_Route->setStyleSheet(buttonStyle);
    ui->button_Map->setStyleSheet(buttonStyle);

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

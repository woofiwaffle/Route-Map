#include "headers/mainwindow.h"
#include "ui_mainwindow.h"

#include "headers/interface.h"



MainWindow::MainWindow(QWidget *parent): QWidget(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    connect(ui->button_Map, &QPushButton::clicked, this, &MainWindow::openMapWindow);
    connect(ui->button_Edit, &QPushButton::clicked, this, &MainWindow::openEditWindow);
}



MainWindow::~MainWindow() {
    delete ui;
}


void MainWindow::openMapWindow() {
    //TInterface *mapWindow = new TInterface();
    //this->hide();

    //mapWindow->show();
}



void MainWindow::openEditWindow() {
    TInterface *editWindow = new TInterface();
    this->hide();
    editWindow->show();
}

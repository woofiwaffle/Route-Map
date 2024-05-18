#include "headers/interfacemap.h"
#include "headers/mainwindow.h"
#include "ui_interfacemap.h"



InterfaceMap::InterfaceMap(QWidget *parent) : QWidget(parent), ui(new Ui::InterfaceMap) {
    ui->setupUi(this);
    setWindowTitle("Map Editor");

    connect(ui->button_Back, &QPushButton::clicked, this, &InterfaceMap::backToMain);
    connect(ui->button_ClearMap, &QPushButton::clicked, this, &InterfaceMap::on_button_ClearMap_clicked);

    scene = new QGraphicsScene(this);   // Инициализируем графическую сцену
    scene->setItemIndexMethod(QGraphicsScene::NoIndex); // настраиваем индексацию элементов

    ui->graphicsView->resize(1000,650);  // Устанавливаем размер graphicsView
    ui->graphicsView->setScene(scene);  // Устанавливаем графическую сцену в graphicsView
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);    // Настраиваем рендер
    ui->graphicsView->setCacheMode(QGraphicsView::CacheBackground); // Кэш фона
    ui->graphicsView->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);


    scene->setSceneRect(ui->graphicsView->x(), ui->graphicsView->y(), 1000, 650); // Устанавливаем размер сцены
}



InterfaceMap::~InterfaceMap() {
    delete ui;
}



void InterfaceMap::backToMain() {
    MainWindow *mainWindow = new MainWindow();
    this->close();
    mainWindow->show();
}



void InterfaceMap::mousePressEvent(QMouseEvent *event) {
    if(event->button() == Qt::LeftButton) {
       QPointF point(-ui->graphicsView->x() + event->pos().x(), -ui->graphicsView->y() + event->pos().y());
       QGraphicsItem* pointItem = new QGraphicsEllipseItem(0, 0, 2, 2);
       pointItem->setPos(point);
       scene->addItem(pointItem);
       obstacle.addPointToPolygon(point);
    }

    if(event->button() == Qt::RightButton) {
       obstacle.finalizePolygon(scene);
    }
}



void InterfaceMap::on_button_ClearMap_clicked() {
    scene->clear();
    obstacle.clearAll();
}



void InterfaceMap::on_button_Save_clicked() {
    obstacle.saveToXml();
}

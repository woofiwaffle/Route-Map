#include "headers/interfacemap.h"
#include "headers/mainwindow.h"
#include "ui_interfacemap.h"



InterfaceMap::InterfaceMap(QWidget *parent) : QWidget(parent), ui(new Ui::InterfaceMap) {
    ui->setupUi(this);
    setWindowTitle("Map Editor");

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
    ui->button_Back->setStyleSheet(buttonStyle);
    ui->button_ClearMap->setStyleSheet(buttonStyle);
    ui->button_Save->setStyleSheet(buttonStyle);

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


// функция возврата на главный экран
void InterfaceMap::backToMain() {
    MainWindow *mainWindow = new MainWindow();
    this->close();
    mainWindow->show();
}


// функция создания препятствия на карте
void InterfaceMap::mousePressEvent(QMouseEvent *event) {
    if(event->button() == Qt::LeftButton) {
       QPointF point(-ui->graphicsView->x() + event->pos().x(), -ui->graphicsView->y() + event->pos().y());
       QGraphicsEllipseItem* pointItem = new QGraphicsEllipseItem(0, 0, 10, 10);

       QPen pen(Qt::white);
       pointItem->setPen(pen);
       pointItem->setPos(point);
       scene->addItem(pointItem);
       obstacle.addPointToPolygon(point);
    }

    if(event->button() == Qt::RightButton) {
       obstacle.finalizePolygon(scene);
    }
}


// функция очистки карты
void InterfaceMap::on_button_ClearMap_clicked() {
    scene->clear();
    obstacle.clearAll();
}


// функция сохранения карты
void InterfaceMap::on_button_Save_clicked() {
    obstacle.saveToXml();
}

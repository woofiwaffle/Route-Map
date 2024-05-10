#include "headers/interfaceroute.h"
#include "headers/mainwindow.h"
#include "ui_interfaceroute.h"
#include "ui_interfacemap.h"



InterfaceRoute::InterfaceRoute(QWidget *parent) : QWidget(parent), ui(new Ui::InterfaceRoute) {
    ui->setupUi(this);
    setWindowTitle("Route");

    connect(ui->button_Back, &QPushButton::clicked, this, &InterfaceRoute::backToMain);
    connect(ui->button_StartJourney, &QPushButton::clicked, this, &InterfaceRoute::on_button_StartJourney_clicked);
    connect(ui->button_LoadingMap, &QPushButton::clicked, this, &InterfaceRoute::on_button_LoadingMap_clicked);
    //connect(ui->button_Save, &QPushButton::clicked, this, &InterfaceRoute::on_button_Save_clicked);



    scene = new QGraphicsScene(this);   // Инициализируем графическую сцену
    scene->setItemIndexMethod(QGraphicsScene::NoIndex); // настраиваем индексацию элементов

    ui->graphicsView->resize(600,600);  // Устанавливаем размер graphicsView
    ui->graphicsView->setScene(scene);  // Устанавливаем графическую сцену в graphicsView
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);    // Настраиваем рендер
    ui->graphicsView->setCacheMode(QGraphicsView::CacheBackground); // Кэш фона
    ui->graphicsView->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);


    scene->setSceneRect(ui->graphicsView->x(), ui->graphicsView->y(), 600, 600); // Устанавливаем размер сцены
}



InterfaceRoute::~InterfaceRoute() {
    delete ui;
}



void InterfaceRoute::backToMain() {
    MainWindow *mainWindow = new MainWindow();
    this->close();
    mainWindow->show();
}



void InterfaceRoute::loadMapFromXml(const QString& fileName) {
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        return;
    }

    QXmlStreamReader reader(&file);

    while(!reader.atEnd() && !reader.hasError()){
        QXmlStreamReader::TokenType token = reader.readNext();
        if(token == QXmlStreamReader::StartElement){
            QXmlStreamAttributes attributes = reader.attributes();
            qreal x = 0, y = 0;
            int passIndex = 0;

            if(reader.name().toString() == "point"){
                if(attributes.hasAttribute("x")){
                    x = attributes.value("x").toString().toDouble();
                }
                if(attributes.hasAttribute("y")){
                    y = attributes.value("y").toString().toDouble();
                }
                QPointF point(x,y);
                Polygon << point;
            }
            if(reader.name().toString() == "index"){
                if (attributes.hasAttribute("id")) {
                    passIndex = attributes.value("id").toInt();
                    indexes.push_back(passIndex);

                    scene->addPolygon(Polygon, QPen(Qt::white), Qt::green);
                    Polygons.push_back(Polygon);

                    QPointF center = (Polygon[0] + Polygon[Polygon.size()-1]) / 2;
                    QGraphicsTextItem* indexItem = scene->addText(QString::number(passIndex));
                    indexItem->setPos(center);
                    indexItem->setDefaultTextColor(Qt::red);
                    Polygon.clear();
                }
            }
        }

    }
    if(reader.hasError()){
        qDebug() << "XML papsing error";
    }
    file.close();
}



void InterfaceRoute::on_button_LoadingMap_clicked() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Map"), "", tr("XML Files (*.xml)"));
    if (fileName.isEmpty()) {
        return;
    }
    if(!Polygons.empty()) Polygons.clear();
    scene->clear();
    loadMapFromXml(fileName);
}



void InterfaceRoute::findOptimalRoute(QGraphicsItem* start, QGraphicsItem* finish) {

}



void InterfaceRoute::on_button_StartJourney_clicked() {

}


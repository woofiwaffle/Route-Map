#include "headers/interfacemap.h"
#include "headers/mainwindow.h"
#include "ui_interfacemap.h"



InterfaceMap::InterfaceMap(QWidget *parent) : QWidget(parent), ui(new Ui::InterfaceMap) {
    ui->setupUi(this);
    setWindowTitle("Map Editor");

    connect(ui->button_Back, &QPushButton::clicked, this, &InterfaceMap::backToMain);
    connect(ui->button_ClearMap, &QPushButton::clicked, this, &InterfaceMap::on_button_ClearMap_clicked);
    connect(ui->button_Save, &QPushButton::clicked, this, &InterfaceMap::on_button_Save_clicked);



    scene = new QGraphicsScene(this);   // Инициализируем графическую сцену
    scene->setItemIndexMethod(QGraphicsScene::NoIndex); // настраиваем индексацию элементов

    ui->graphicsView->resize(600,600);  // Устанавливаем размер graphicsView
    ui->graphicsView->setScene(scene);  // Устанавливаем графическую сцену в graphicsView
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);    // Настраиваем рендер
    ui->graphicsView->setCacheMode(QGraphicsView::CacheBackground); // Кэш фона
    ui->graphicsView->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);


    scene->setSceneRect(ui->graphicsView->x(), ui->graphicsView->y(), 600, 600); // Устанавливаем размер сцены
}



InterfaceMap::~InterfaceMap() {
    delete ui;
}



void InterfaceMap::backToMain() {
    MainWindow *mainWindow = new MainWindow();
    this->close();
    mainWindow->show();
}



void InterfaceMap::mousePressEvent(QMouseEvent *event){
    QPen pen(Qt::white);

    if(event->button() == Qt::LeftButton){
        QPointF point( - ui->graphicsView->x() + event->pos().x(), - ui->graphicsView->y() + event->pos().y());
        QGraphicsItem* pointItem = new QGraphicsEllipseItem(0,0,2,2);
        pointItem->setPos( - ui->graphicsView->x() + event->pos().x(), - ui->graphicsView->y() + event->pos().y());
        scene->addItem(pointItem);
        Polygon << point;
    }
    if(event->button() == Qt::RightButton){
        for( QGraphicsItem* item : scene->items()){
            if(QGraphicsEllipseItem *Item = dynamic_cast<QGraphicsEllipseItem *>(item)){
                delete Item;
            }
        }
        scene->addPolygon(Polygon, pen, Qt::green);

        Polygons.push_back(Polygon);

        bool ok;
        int passIndex = QInputDialog::getInt(this, tr("Create Index"), tr("Enter index (0 - 100)"), 0, 0, 100, 1, &ok);
        if(ok){
            //for(QGraphicsItem* item : scene->items()){
            //    QGraphicsLineItem* lineItem = dynamic_cast<QGraphicsLineItem*>(item);
            //    if(lineItem){
            //
            //        lineItem->setData(passIndexRole, passIndex);
            //    }
            //}
            QPointF center = (Polygon[0] + Polygon[Polygon.size()-1]) / 2;
            QGraphicsTextItem* indexItem = scene->addText(QString::number(passIndex));
            indexItem->setPos(center);
            indexItem->setDefaultTextColor(Qt::red);
            indexes.push_back(passIndex);
        }
        Polygon.clear();
    }

}



void InterfaceMap::on_button_ClearMap_clicked() {
    scene->clear();
    Polygon.clear();
    Polygons.clear();
    indexes.clear();
}



void InterfaceMap::on_button_Save_clicked(){
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Map"), "", tr("XML Files (*.xml)"));
    if(fileName.isEmpty()){
        return;
    }

    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly)){
        qDebug() << "Failed to open file for writing";
        return;
    }

    QXmlStreamWriter xmlWriter(&file);
    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartDocument();
    xmlWriter.writeStartElement("map");

    for(int i = 0; i < Polygons.size(); i++){
        xmlWriter.writeStartElement("obstacle");
        for(QPointF item : Polygons[i]){
            xmlWriter.writeStartElement("point");
            xmlWriter.writeAttribute("x", QString::number(item.x()));
            xmlWriter.writeAttribute("y", QString::number(item.y()));
            xmlWriter.writeEndElement();
        }
        xmlWriter.writeStartElement("index");
        int passIndex = indexes[i];
        xmlWriter.writeAttribute("id", QString::number(passIndex));
        xmlWriter.writeEndElement();
        xmlWriter.writeEndElement();
    }

    xmlWriter.writeEndElement();
    xmlWriter.writeEndDocument();
    file.close();

    qDebug() << "Map save to" << fileName;
}

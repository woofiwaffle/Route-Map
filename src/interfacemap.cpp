#include "headers/interfacemap.h"
#include "headers/mainwindow.h"
#include "headers/moveitem.h"
#include "headers/obstacle.h"

#include "ui_interfacemap.h"

InterfaceMap::InterfaceMap(QWidget *parent) : QWidget(parent), ui(new Ui::InterfaceMap) {
    ui->setupUi(this);
    setWindowTitle("Map Editor");

    connect(ui->button_Back, &QPushButton::clicked, this, &InterfaceMap::backToMain);
    connect(ui->button_CreateLine, &QPushButton::clicked, this, &InterfaceMap::on_button_CreateLine_clicked);
    connect(ui->button_ClearMap, &QPushButton::clicked, this, &InterfaceMap::on_button_ClearMap_clicked);
    connect(ui->button_Save, &QPushButton::clicked, this, &InterfaceMap::on_button_Save_clicked);



    scene = new QGraphicsScene(this);   // Инициализируем графическую сцену
    scene->setItemIndexMethod(QGraphicsScene::NoIndex); // настраиваем индексацию элементов

    //ui->graphicsView->resize(600,600);  // Устанавливаем размер graphicsView
    ui->graphicsView->setScene(scene);  // Устанавливаем графическую сцену в graphicsView
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);    // Настраиваем рендер
    ui->graphicsView->setCacheMode(QGraphicsView::CacheBackground); // Кэш фона
    ui->graphicsView->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);


    scene->setSceneRect(112, 33, 550, 550); // Устанавливаем размер сцены


}



InterfaceMap::~InterfaceMap() {
    delete ui;
}



void InterfaceMap::mousePressEvent(QMouseEvent *event){
    MoveItem *item = new MoveItem();  // Создаём графический элемент
    if(event->button() == Qt::LeftButton){
        item->setPos(event->pos());
        scene->addItem(item);   // Добавляем элемент на графическую сцену
    }
    connect(item, &MoveItem::pointAdded, this, &InterfaceMap::on_button_CreateLine_clicked);
    //connect(item, &MoveItem::pointAdded, this, &TInterface::updateLine);
}



void InterfaceMap::mouseMoveEvent(QMouseEvent *event){

}



void InterfaceMap::mouseReleaseEvent(QMouseEvent *event){

}



void InterfaceMap::backToMain() {
    MainWindow *mainWindow = new MainWindow();
    this->close();
    mainWindow->show();
}

bool InterfaceMap::search(QGraphicsItem* item){
    if(Points.empty()) return false;
    for(int i = Points.size()-1; i >= 0; i--){
        if(item == Points[i]) {
            return true;
        }
    }
    return false;
}

void InterfaceMap::on_button_CreateLine_clicked() {
    QPen pen(Qt::white);
    if(scene->items().size() >= 2){
        vector<QGraphicsItem*> Polygon;
        MoveItem *item1 = nullptr;
        MoveItem *item2 = nullptr;
        MoveItem *root = nullptr;
        for(QGraphicsItem *item : scene->items()){
            if(!search(item)){
                if(MoveItem *moveItem = dynamic_cast<MoveItem *>(item)){
                    if(root == nullptr){
                        root = dynamic_cast<MoveItem *>(item);
                        Points.push_back(item);
                        Polygon.push_back(item);
                    }
                    else{
                        if(item1 == nullptr){
                            item1 = moveItem;
                            Points.push_back(item);
                            Polygon.push_back(item);
                            QGraphicsLineItem *line = new QGraphicsLineItem(QLineF(root->pos(), item1->pos()));
                            line->setPen(pen);
                            scene->addItem(line);
                        }
                        else{
                            item2 = moveItem;
                            Points.push_back(item);
                            Polygon.push_back(item);
                            QGraphicsLineItem *line = new QGraphicsLineItem(QLineF(item1->pos(), item2->pos()));
                            line->setPen(pen);
                            scene->addItem(line);
                            item1 = item2;
                            item2 = nullptr;
                        }
                    }

                }
            }
        }
        Polygons.push_back(Polygon);
        if(item1 != nullptr && root != nullptr){
            QGraphicsLineItem *line = new QGraphicsLineItem(QLineF(root->pos(), item1->pos()));
            line->setPen(pen);
            scene->addItem(line);

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
                QPointF center = (root->pos() + item1->pos()) / 2;
                QGraphicsTextItem* indexItem = scene->addText(QString::number(passIndex));
                indexItem->setPos(center);
                indexItem->setDefaultTextColor(Qt::blue);
                indexes.push_back(passIndex);
            }
        }
    }
}



void InterfaceMap::on_button_ClearMap_clicked() {
    scene->clear();
    Points.clear();
    Polygons.clear();
    indexes.clear();
}



void InterfaceMap::on_button_CreateIndex_clicked() {
//    QGraphicsItem* selectedItem = scene->selectedItems().first();
//    MoveItem* selectedMoveItem = dynamic_cast<MoveItem*>(selectedItem);
//    if(!selectedMoveItem){
//        return;
//    }
}



void InterfaceMap::on_button_Save_clicked(){
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Map"), "", tr("XML Files (*.xml)"));
    if(fileName.isEmpty()){
        return;
    }

    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly)){
        qDebug() << "Failed to open file for writing";                               // этот парсинг не записыывает линии
        return;
    }

    QXmlStreamWriter xmlWriter(&file);
    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartDocument();
    xmlWriter.writeStartElement("map");

    int flag = 0; // обозначает, когда писать индекс плотности (то есть в конец препятствия)

    QList<QGraphicsItem*> items = scene->items();
    for(int i = 0; i < Polygons.size(); i+=2){
        xmlWriter.writeStartElement("obstacle");
        for(QGraphicsItem* item : Polygons[i]){
            if(MoveItem* moveItem = dynamic_cast<MoveItem *>(item)){
                QPointF pos = item->pos();

                xmlWriter.writeStartElement("point");
                xmlWriter.writeAttribute("x", QString::number(pos.x()));
                xmlWriter.writeAttribute("y", QString::number(pos.y()));
                xmlWriter.writeEndElement();
            }
        }
        xmlWriter.writeStartElement("index");
        int passIndex = indexes[i/2];
        xmlWriter.writeAttribute("id", QString::number(passIndex));
        xmlWriter.writeEndElement();
        flag = 0;
        xmlWriter.writeEndElement();
    }

    xmlWriter.writeEndElement();
    xmlWriter.writeEndDocument();
    file.close();

    qDebug() << "Map save to" << fileName;
}

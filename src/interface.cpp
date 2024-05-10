#include "headers/interface.h"
#include "headers/mainwindow.h"
#include "headers/moveitem.h"
#include "headers/obstacle.h"
#include "ui_interface.h"

TInterface::TInterface(QWidget *parent) : QWidget(parent), ui(new Ui::TInterface) {
    ui->setupUi(this);
    setWindowTitle("Map Editor");

    connect(ui->button_Back, &QPushButton::clicked, this, &TInterface::backToMain);
    connect(ui->button_CreateLine, &QPushButton::clicked, this, &TInterface::on_button_CreateLine_clicked);
    connect(ui->button_ClearMap, &QPushButton::clicked, this, &TInterface::on_button_ClearMap_clicked);
    connect(ui->button_Save, &QPushButton::clicked, this, &TInterface::on_button_Save_clicked);



    scene = new QGraphicsScene(this);   // Инициализируем графическую сцену
    scene->setItemIndexMethod(QGraphicsScene::NoIndex); // настраиваем индексацию элементов

    //ui->graphicsView->resize(600,600);  // Устанавливаем размер graphicsView
    ui->graphicsView->setScene(scene);  // Устанавливаем графическую сцену в graphicsView
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);    // Настраиваем рендер
    ui->graphicsView->setCacheMode(QGraphicsView::CacheBackground); // Кэш фона
    ui->graphicsView->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);


    scene->setSceneRect(112, 33, 550, 550); // Устанавливаем размер сцены


}



TInterface::~TInterface() {
    delete ui;
}



void TInterface::mousePressEvent(QMouseEvent *event){
    MoveItem *item = new MoveItem();  // Создаём графический элемент
    if(event->button() == Qt::LeftButton){
        item->setPos(event->pos());
        scene->addItem(item);   // Добавляем элемент на графическую сцену
    }
    connect(item, &MoveItem::pointAdded, this, &TInterface::on_button_CreateLine_clicked);
    //connect(item, &MoveItem::pointAdded, this, &TInterface::updateLine);
}



void TInterface::mouseMoveEvent(QMouseEvent *event){

}



void TInterface::mouseReleaseEvent(QMouseEvent *event){

}



void TInterface::backToMain() {
    MainWindow *mainWindow = new MainWindow();
    this->close();
    mainWindow->show();
}

bool TInterface::search(QGraphicsItem* item){
    if(Points.empty()) return false;
    for(int i = Points.size()-1; i >= 0; i--){
        if(item == Points[i]) {
            return true;
        }
    }
    return false;
}

void TInterface::on_button_CreateLine_clicked() {
    QPen pen(Qt::white);
    if(scene->items().size() >= 2){
        MoveItem *item1 = nullptr;
        MoveItem *item2 = nullptr;
        MoveItem *root = nullptr;
        for(QGraphicsItem *item : scene->items()){
            if(!search(item)){
                if(MoveItem *moveItem = dynamic_cast<MoveItem *>(item)){
                    if(root == nullptr){
                        root = dynamic_cast<MoveItem *>(item);
                        Points.push_back(item);
                    }
                    else{
                        if(item1 == nullptr){
                            item1 = moveItem;
                            Points.push_back(item);
                            QGraphicsLineItem *line = new QGraphicsLineItem(QLineF(root->pos(), item1->pos()));
                            line->setPen(pen);
                            scene->addItem(line);
                        }
                        else{
                            item2 = moveItem;
                            Points.push_back(item);
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
            }
        }
    }
}



void TInterface::on_button_ClearMap_clicked() {
    scene->clear();
    Points.clear();
}



void TInterface::updateLine() {
    QList<QGraphicsItem *> items = scene->items();
    for(QGraphicsItem *item : items){
        QGraphicsLineItem *line = dynamic_cast<QGraphicsLineItem *>(item);
        if(line){
            QList<QGraphicsItem *> lineItems = line->collidingItems();
            for(QGraphicsItem *lineItem : lineItems){
                MoveItem *moveItem = dynamic_cast<MoveItem *>(lineItem);
//                if(moveItem && (moveItem == item1 || moveItem == item2)){
//                    line->setLine(QLineF(item1->pos(), item2->pos()));
//                    break;
//                }
            }
        }
    }
}



void TInterface::on_button_CreateIndex_clicked() {
//    QGraphicsItem* selectedItem = scene->selectedItems().first();
//    MoveItem* selectedMoveItem = dynamic_cast<MoveItem*>(selectedItem);
//    if(!selectedMoveItem){
//        return;
//    }
}



void TInterface::on_button_Save_clicked(){
//    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Map"), "", tr("XML Files (*.xml)"));
//    if(fileName.isEmpty()){
//        return;
//    }

//    QFile file(fileName);
//    if(!file.open(QIODevice::WriteOnly)){
//        qDebug() << "Failed to open file for writing";
//        return;
//    }

//    QXmlStreamWriter xmlWriter(&file);
//    xmlWriter.setAutoFormatting(true);
//    xmlWriter.writeStartDocument();
//    xmlWriter.writeStartElement("map");

//    QList<QGraphicsItem*> items = scene->items();
//    for(QGraphicsItem* item : items){
//        Obstacle* obstacle = dynamic_cast<Obstacle*>(item);
//        if(obstacle){
//            QPointF pos = item->pos();
//            int passIndex = obstacle->passIndex();

//            xmlWriter.writeStartElement("obstacle");
//            xmlWriter.writeAttribute("x", QString::number(pos.x()));
//            xmlWriter.writeAttribute("y", QString::number(pos.y()));
//            xmlWriter.writeAttribute("passIndex", QString::number(passIndex));
//            xmlWriter.writeEndElement();
//        }
//    }
//    xmlWriter.writeEndElement();
//    xmlWriter.writeEndDocument();
//    file.close();

//    qDebug() << "Map save to" << fileName;
}

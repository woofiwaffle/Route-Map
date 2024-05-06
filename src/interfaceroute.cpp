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

    //ui->graphicsView->resize(600,600);  // Устанавливаем размер graphicsView
    ui->graphicsView->setScene(scene);  // Устанавливаем графическую сцену в graphicsView
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);    // Настраиваем рендер
    ui->graphicsView->setCacheMode(QGraphicsView::CacheBackground); // Кэш фона
    ui->graphicsView->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);


    scene->setSceneRect(112, 33, 550, 550); // Устанавливаем размер сцены
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
    std::vector<QGraphicsItem*> Polygon;
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
                MoveItem *moveItem = new MoveItem();
                Polygon.push_back(moveItem);
                moveItem->setPos(x, y);

                scene->addItem(moveItem);
            }
            if(reader.name().toString() == "index"){
                if (attributes.hasAttribute("id")) {
                    passIndex = attributes.value("id").toInt();
                    indexes.push_back(passIndex);
                    Polygons.push_back(Polygon);
                    Polygon.clear();
                }
                //moveItem->setPassIndex(passIndex);
            }
        }
    }
    if(reader.hasError()){
        qDebug() << "XML papsing error";
    }

    /*QPainter *painter;
    QPolygon Pol;
    for(int i = 0; i < Polygons.size(); i++){
        for(QGraphicsItem* item : Polygons[i]){
            QPoint* Item = dynamic_cast<QPoint* >(item);
            Pol << *Item;
        }
        painter->drawPolygon(Pol);
    }*/




    // Создание линий на карте маршрута
    QPen pen(Qt::white);
    for(size_t i = 0; i < Polygons.size(); i+=1){
        MoveItem *item1 = nullptr;
        MoveItem *item2 = nullptr;
        MoveItem *root = nullptr;
        for(QGraphicsItem* item : Polygons[i]){
            if(root == nullptr){
                root = dynamic_cast<MoveItem *>(item);
            }
            else{
                if(item1 == nullptr){
                    item1 = dynamic_cast<MoveItem *>(item);;
                    QGraphicsLineItem *line = new QGraphicsLineItem(QLineF(root->pos(), item1->pos()));
                    line->setPen(pen);
                    scene->addItem(line);
                }
                else{
                    item2 = dynamic_cast<MoveItem *>(item);
                    QGraphicsLineItem *line = new QGraphicsLineItem(QLineF(item1->pos(), item2->pos()));
                    line->setPen(pen);
                    scene->addItem(line);
                    item1 = item2;
                    item2 = nullptr;
                }
            }
        }
        if(item1 != nullptr && root != nullptr){
            QGraphicsLineItem *line = new QGraphicsLineItem(QLineF(root->pos(), item1->pos()));
            line->setPen(pen);
            scene->addItem(line);

//            bool ok;
//            int passIndex = indexes[i];
//            if(ok){
                //for(QGraphicsItem* item : scene->items()){
                //    QGraphicsLineItem* lineItem = dynamic_cast<QGraphicsLineItem*>(item);
                //    if(lineItem){
                //
                //        lineItem->setData(passIndexRole, passIndex);
                //    }
                //}
                QPointF center = (root->pos() + item1->pos()) / 2;
                QGraphicsTextItem* indexItem = scene->addText(QString::number(indexes[i]));
                indexItem->setPos(center);
                indexItem->setDefaultTextColor(Qt::red);
                //indexes.push_back(passIndex);
            //}
        }
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



void InterfaceMap::mousePressEvent(QMouseEvent *event){
}

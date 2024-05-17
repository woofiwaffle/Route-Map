//#include "headers/route.h"



//Route::Route(QObject *parent) : QObject(parent), scene(nullptr) {}



//void Route::loadMapFromXml(const QString& fileName) {
//    vector<QGraphicsItem*> Polygon;
//    QFile file(fileName);
//    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
//        return;
//    }

//    QXmlStreamReader reader(&file);

//    while(!reader.atEnd() && !reader.hasError()){
//        QXmlStreamReader::TokenType token = reader.readNext();
//        if(token == QXmlStreamReader::StartElement){
//            QXmlStreamAttributes attributes = reader.attributes();
//            qreal x = 0, y = 0;
//            int passIndex = 0;

//            if(reader.name().toString() == "point"){
//                if(attributes.hasAttribute("x")){
//                    x = attributes.value("x").toString().toDouble();
//                }
//                if(attributes.hasAttribute("y")){
//                    y = attributes.value("y").toString().toDouble();
//                }
//                MoveItem *moveItem = new MoveItem();
//                Polygon.push_back(moveItem);
//                moveItem->setPos(x, y);

//                scene->addItem(moveItem);
//            }
//            if(reader.name().toString() == "index"){
//                if (attributes.hasAttribute("id")) {
//                    passIndex = attributes.value("id").toInt();
//                    indexes.push_back(passIndex);
//                    Polygons.push_back(Polygon);
//                    Polygon.clear();
//                }
//                //moveItem->setPassIndex(passIndex);
//            }
//        }
//    }
//    if(reader.hasError()){
//        qDebug() << "XML papsing error";
//    }

//    /*QPainter *painter;
//    QPolygon Pol;
//    for(int i = 0; i < Polygons.size(); i++){
//        for(QGraphicsItem* item : Polygons[i]){
//            QPoint* Item = dynamic_cast<QPoint* >(item);
//            Pol << *Item;
//        }
//        painter->drawPolygon(Pol);
//    }*/




//    // Создание линий на карте маршрута
//    QPen pen(Qt::white);
//    for(size_t i = 0; i < Polygons.size(); i+=1){
//        MoveItem *item1 = nullptr;
//        MoveItem *item2 = nullptr;
//        MoveItem *root = nullptr;
//        for(QGraphicsItem* item : Polygons[i]){
//            if(root == nullptr){
//                root = dynamic_cast<MoveItem *>(item);
//            }
//            else{
//                if(item1 == nullptr){
//                    item1 = dynamic_cast<MoveItem *>(item);;
//                    QGraphicsLineItem *line = new QGraphicsLineItem(QLineF(root->pos(), item1->pos()));
//                    line->setPen(pen);
//                    scene->addItem(line);
//                }
//                else{
//                    item2 = dynamic_cast<MoveItem *>(item);
//                    QGraphicsLineItem *line = new QGraphicsLineItem(QLineF(item1->pos(), item2->pos()));
//                    line->setPen(pen);
//                    scene->addItem(line);
//                    item1 = item2;
//                    item2 = nullptr;
//                }
//            }
//        }
//        if(item1 != nullptr && root != nullptr){
//            QGraphicsLineItem *line = new QGraphicsLineItem(QLineF(root->pos(), item1->pos()));
//            line->setPen(pen);
//            scene->addItem(line);

////            bool ok;
////            int passIndex = indexes[i];
////            if(ok){
//                //for(QGraphicsItem* item : scene->items()){
//                //    QGraphicsLineItem* lineItem = dynamic_cast<QGraphicsLineItem*>(item);
//                //    if(lineItem){
//                //
//                //        lineItem->setData(passIndexRole, passIndex);
//                //    }
//                //}
//                QPointF center = (root->pos() + item1->pos()) / 2;
//                QGraphicsTextItem* indexItem = scene->addText(QString::number(indexes[i]));
//                indexItem->setPos(center);
//                indexItem->setDefaultTextColor(Qt::red);
//                //indexes.push_back(passIndex);
//            //}
//        }
//    }





//    file.close();
//}



//void Route::setStartPoint(QGraphicsItem startPoint) {

//}



//void Route::setFinishPoint(QGraphicsItem finishPoint) {

//}



//void Route::findOptimalRoute() {

//}



//void Route::saveRouteToFile(const QString &fileName) {

//}



//void Route::clearRoute() {

//}

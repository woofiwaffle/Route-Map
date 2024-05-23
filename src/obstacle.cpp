#include "headers/obstacle.h"



void Obstacle::addPointToPolygon(const QPointF &point) {
    Polygon << point;
}



void Obstacle::finalizePolygon(QGraphicsScene *scene) {
    QPen pen(Qt::white);

    for (QGraphicsItem* item : scene->items()) {
        if (QGraphicsEllipseItem *Item = dynamic_cast<QGraphicsEllipseItem *>(item)) {
            delete Item;
        }
    }
    scene->addPolygon(Polygon, pen, Qt::lightGray);
    Polygons.push_back(Polygon);

    bool ok;
    int passIndex = QInputDialog::getInt(nullptr, "Create Index", "Enter index (1 - 100)", 0, 1, 100, 1, &ok);
    if (ok) {
        // Вычисляем центроид многоугольника;
        QPointF centroid(0, 0);
        double signedArea = 0.0;
        double x0 = 0.0; //x-координата текущей вершины
        double y0 = 0.0; // координата y текущей вершины
        double x1 = 0.0; // координата x следующей вершины
        double y1 = 0.0; // координата y следующей вершины
        double a = 0.0;  // Частичная подписанная область

        for (int i = 0; i < Polygon.size(); ++i) {
            x0 = Polygon[i].x();
            y0 = Polygon[i].y();
            x1 = Polygon[(i + 1) % Polygon.size()].x();
            y1 = Polygon[(i + 1) % Polygon.size()].y();
            a = x0 * y1 - x1 * y0;
            signedArea += a;
            centroid.rx() += (x0 + x1) * a;
            centroid.ry() += (y0 + y1) * a;
        }

        signedArea *= 0.5;
        centroid.rx() /= (6.0 * signedArea);
        centroid.ry() /= (6.0 * signedArea);

        QGraphicsTextItem* indexItem = scene->addText(QString::number(passIndex));
        indexItem->setPos(centroid);

         // Установка цвета в зависимости от значения индекса
        if(passIndex <= 40){
           indexItem->setDefaultTextColor(Qt::darkGreen);  // Зелёный для значений <= 40
        }
        else if(passIndex <= 70){
            indexItem->setDefaultTextColor(Qt::darkYellow); // Жёлтый для значений 41-70
        }
        else{
            indexItem->setDefaultTextColor(Qt::darkRed);    // Красный для значений > 70
        }
        indexes.push_back(passIndex);
    }
    else {
        scene->removeItem(scene->items().first());
        Polygons.pop_back();
    }
    Polygon.clear();
}



void Obstacle::clearAll() {
    Polygon.clear();
    Polygons.clear();
    indexes.clear();
}



void Obstacle::saveToXml() {
    QString fileName = QFileDialog::getSaveFileName(nullptr, "Save Map", "", "XML Files (*.xml)");
    if (fileName.isEmpty()) {
        return;
    }
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::warning(nullptr, "Ошибка", "Не удалось открыть файл для записи.");
        return;
    }

    QXmlStreamWriter xmlWriter(&file);
    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartDocument();
    xmlWriter.writeStartElement("map");

    for (int i = 0; i < Polygons.size(); i++) {
        xmlWriter.writeStartElement("obstacle");
        for (const QPointF& item : Polygons[i]) {
            xmlWriter.writeStartElement("point");
            xmlWriter.writeAttribute("x", QString::number(item.x()));
            xmlWriter.writeAttribute("y", QString::number(item.y()));
            xmlWriter.writeEndElement();
        }
        xmlWriter.writeStartElement("index");
        int passIndex = indexes[i];
        xmlWriter.writeAttribute("id", QString::number(passIndex));
        xmlWriter.writeEndElement();
        xmlWriter.writeEndElement(); // obstacle
    }

    xmlWriter.writeEndElement(); // map
    xmlWriter.writeEndDocument();
    file.close();

    QMessageBox::information(nullptr, "Сохранение завершено", "Карта успешно сохранена в файл.");
}

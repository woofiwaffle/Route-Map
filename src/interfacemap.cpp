#include "headers/interfacemap.h"
#include "headers/mainwindow.h"
#include "ui_interfacemap.h"



InterfaceMap::InterfaceMap(QWidget *parent) : QWidget(parent), ui(new Ui::InterfaceMap) {
    ui->setupUi(this);
    setWindowTitle("Map Editor");

    connect(ui->button_Back, &QPushButton::clicked, this, &InterfaceMap::backToMain);
    connect(ui->button_ClearMap, &QPushButton::clicked, this, &InterfaceMap::on_button_ClearMap_clicked);
    //connect(ui->button_Save, &QPushButton::clicked, this, &InterfaceMap::on_button_Save_clicked); закоментировал так как вызывается почему то два раза



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
    QPen pen(Qt::white);

    if(event->button() == Qt::LeftButton) {
        QPointF point(-ui->graphicsView->x() + event->pos().x(), -ui->graphicsView->y() + event->pos().y());
        QGraphicsItem* pointItem = new QGraphicsEllipseItem(0, 0, 2, 2);
        pointItem->setPos(point);
        scene->addItem(pointItem);
        Polygon << point;
    }

    if(event->button() == Qt::RightButton) {
        for (QGraphicsItem* item : scene->items()) {
            if (QGraphicsEllipseItem *Item = dynamic_cast<QGraphicsEllipseItem *>(item)) {
                delete Item;
            }
        }
        scene->addPolygon(Polygon, pen, Qt::lightGray);
        Polygons.push_back(Polygon);

        bool ok;
        int passIndex = QInputDialog::getInt(this, tr("Create Index"), tr("Enter index (1 - 100)"), 0, 0, 100, 1, &ok);
        if (ok) {
            // Вычисляем центр масс (центроид) многоугольника
            QPointF centroid(0, 0);
            double signedArea = 0.0;
            double x0 = 0.0; // x координата текущей вершины
            double y0 = 0.0; // y координата текущей вершины
            double x1 = 0.0; // x координата следующей вершины
            double y1 = 0.0; // y координата следующей вершины
            double a = 0.0;  // Промежуточная переменная

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

            // Добавляем текст с индексом в центр масс
            QGraphicsTextItem* indexItem = scene->addText(QString::number(passIndex));
            indexItem->setPos(centroid);
            indexItem->setDefaultTextColor(Qt::blue);
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



void InterfaceMap::on_button_Save_clicked() {
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Map"), "", tr("XML Files (*.xml)"));
    if (fileName.isEmpty()) {
        return;
    }

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::warning(this, tr("Ошибка"), tr("Не удалось открыть файл для записи."));
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

    QMessageBox::information(this, tr("Сохранение завершено"), tr("Карта успешно сохранена в файл."));
}

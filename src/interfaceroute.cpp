#include "headers/interfaceroute.h"
#include "headers/mainwindow.h"
#include "ui_interfaceroute.h"
#include "ui_interfacemap.h"



InterfaceRoute::InterfaceRoute(QWidget *parent) : QWidget(parent), ui(new Ui::InterfaceRoute) {
    ui->setupUi(this);
    setWindowTitle("Route");

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
    ui->button_LoadingMap->setStyleSheet(buttonStyle);
    ui->button_ClearWay->setStyleSheet(buttonStyle);
    ui->button_StartJourney->setStyleSheet(buttonStyle);
    ui->button_Save->setStyleSheet(buttonStyle);


    connect(ui->button_Back, &QPushButton::clicked, this, &InterfaceRoute::backToMain);
    connect(ui->button_StartJourney, &QPushButton::clicked, this, &InterfaceRoute::on_button_StartJourney_clicked);


    scene = new QGraphicsScene(this);   // Инициализируем графическую сцену
    scene->setItemIndexMethod(QGraphicsScene::NoIndex); // настраиваем индексацию элементов

    ui->graphicsView->resize(1000,650);  // Устанавливаем размер graphicsView
    ui->graphicsView->setScene(scene);  // Устанавливаем графическую сцену в graphicsView
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);    // Настраиваем рендер
    ui->graphicsView->setCacheMode(QGraphicsView::CacheBackground); // Кэш фона
    ui->graphicsView->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);


    scene->setSceneRect(ui->graphicsView->x(), ui->graphicsView->y(), 1000, 650); // Устанавливаем размер сцены

}



InterfaceRoute::~InterfaceRoute() {
    delete ui;
}



void InterfaceRoute::backToMain() {
    MainWindow *mainWindow = new MainWindow();
    this->close();
    mainWindow->show();
}


// функция загрузки карты из XML файла
void InterfaceRoute::on_button_LoadingMap_clicked() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Map"), "", tr("XML Files (*.xml)"));
    if (fileName.isEmpty()) {
        return;
    }
    if(!route.Polygons.empty()) route.Polygons.clear();
    scene->clear();
    StartPoint = nullptr;
    FinishPoint = nullptr;
    route.loadMapFromXml(fileName, scene);
}


// функция вывода маршрута на карте
void InterfaceRoute::on_button_StartJourney_clicked() {
    if (StartPoint && FinishPoint) {
        // Проверяем, находятся ли стартовая и конечная точки внутри препятствий
        if (route.searchPoint(*StartPoint) || route.searchPoint(*FinishPoint)) {
            qDebug() << "Start or finish point cannot be set on an obstacle";
            return;
        }

        Node Start(StartPoint->x(), StartPoint->y(), 0, route.heuristic(StartPoint->x(), StartPoint->y(), FinishPoint->x(), FinishPoint->y()));
        Node Finish(FinishPoint->x(), FinishPoint->y(), 0, 0);
        std::vector<Node> path = route.aStar(Start, Finish, n);
        if (path.empty()) {
            path = route.aStar(Finish, Start, n);
        }
        route.WayPoints = path;

        qDebug() << route.WayPoints.size();

        QPen routePen(Qt::darkCyan, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);

        for (int i = 0; i < route.WayPoints.size() - 1; i++) {
            QLineF line(route.WayPoints[i].Point.x(), route.WayPoints[i].Point.y(), route.WayPoints[i + 1].Point.x(), route.WayPoints[i + 1].Point.y());
            scene->addLine(line, routePen);
        }

        // Отображаем стартовую точку
        QGraphicsEllipseItem *startEllipse = new QGraphicsEllipseItem(StartPoint->x() - 4, StartPoint->y() - 4, 15, 15);
        startEllipse->setBrush(Qt::green);
        scene->addItem(startEllipse);

        // Отображаем конечную точку
        QGraphicsEllipseItem *finishEllipse = new QGraphicsEllipseItem(FinishPoint->x() - 4, FinishPoint->y() - 4, 15, 15);
        finishEllipse->setBrush(Qt::red);
        scene->addItem(finishEllipse);
    }
    else {
        qDebug() << "Start and/or finish point not set";
    }
}



void InterfaceRoute::on_button_ClearWay_clicked(){
    for(QGraphicsItem* item : scene->items()){
        QGraphicsEllipseItem* PointItem = dynamic_cast<QGraphicsEllipseItem*>(item);
        QGraphicsLineItem* lineItem = dynamic_cast<QGraphicsLineItem*>(item);
        if(PointItem){
            delete item;
        }
        else if(lineItem){
            delete item;
        }
    }
    route.WayPoints.clear();
    StartPoint = nullptr;
    FinishPoint = nullptr;
}



void InterfaceRoute::on_button_Save_clicked() {
    if(route.WayPoints.empty()){
        QMessageBox::warning(this, tr("Ошибка"), tr("Нет маршрута для сохранения."));
        return;
    }

    QString fileName = QFileDialog::getSaveFileName(this, tr("Сохранить маршрут"), "", tr("XML файлы (*.xml)"));
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
    xmlWriter.writeStartElement("Route");

    double totalDistance = 0.0;
    double totalTime = 0.0;
    const double averageSpeed = 5.0; // Средняя скорость в м/с (примерное значение)

    xmlWriter.writeStartElement("WayPoints");
    for (size_t i = 0; i < route.WayPoints.size(); ++i) {
        Node& node = route.WayPoints[i];
        xmlWriter.writeStartElement("WayPoint");
        xmlWriter.writeAttribute("x", QString::number(node.Point.x()));
        xmlWriter.writeAttribute("y", QString::number(node.Point.y()));
        xmlWriter.writeAttribute("cost", QString::number(node.cost));
        xmlWriter.writeAttribute("heuristic", QString::number(node.heuristic));
        xmlWriter.writeEndElement(); // WayPoint

        if (i > 0) {
            totalDistance += std::distance(&route.WayPoints[i-1].Point, &node.Point);
        }
    }
    xmlWriter.writeEndElement(); // WayPoints

    totalTime = totalDistance / averageSpeed;

    xmlWriter.writeStartElement("Statistics");
    xmlWriter.writeTextElement("TotalDistance", QString::number(totalDistance));
    xmlWriter.writeTextElement("TotalTime", QString::number(totalTime));
    xmlWriter.writeTextElement("AverageSpeed", QString::number(averageSpeed));
    xmlWriter.writeEndElement(); // Statistics

    xmlWriter.writeEndElement(); // Route
    xmlWriter.writeEndDocument();

    file.close();
    QMessageBox::information(this, tr("Сохранение завершено"), tr("Маршрут успешно сохранен в файл."));
}


// функция установки точкек Старта и Финиша на карте
void InterfaceRoute::mousePressEvent(QMouseEvent *event){
    if(event->button() == Qt::LeftButton){
        if(!StartPoint){
            StartPoint = new QPointF( - ui->graphicsView->x() + event->pos().x(), - ui->graphicsView->y() + event->pos().y());
            QGraphicsEllipseItem* pointItem = new QGraphicsEllipseItem(0,0,10,10);
            QPen pen(Qt::green);
            pointItem->setPen(pen);
            pointItem->setPos( - ui->graphicsView->x() + event->pos().x(), - ui->graphicsView->y() + event->pos().y());
            scene->addItem(pointItem);
            qDebug() << "Start point set";
        }
        else if(!FinishPoint){
            FinishPoint = new QPointF( - ui->graphicsView->x() + event->pos().x(), - ui->graphicsView->y() + event->pos().y());
            QGraphicsEllipseItem* pointItem = new QGraphicsEllipseItem(0,0,10,10);
            QPen pen(Qt::red);
            pointItem->setPen(pen);
            pointItem->setPos( - ui->graphicsView->x() + event->pos().x(), - ui->graphicsView->y() + event->pos().y());
            scene->addItem(pointItem);
            qDebug() << "Finish point set";
        }
        else{
            qDebug() << "Both points already set";
        }
    }
}


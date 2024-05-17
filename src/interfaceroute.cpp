#include "headers/interfaceroute.h"
#include "headers/mainwindow.h"
#include "ui_interfaceroute.h"
#include "ui_interfacemap.h"



InterfaceRoute::InterfaceRoute(QWidget *parent) : QWidget(parent), ui(new Ui::InterfaceRoute) {
    ui->setupUi(this);
    setWindowTitle("Route");

    connect(ui->button_Back, &QPushButton::clicked, this, &InterfaceRoute::backToMain);
    connect(ui->button_StartJourney, &QPushButton::clicked, this, &InterfaceRoute::on_button_StartJourney_clicked);
    //connect(ui->button_LoadingMap, &QPushButton::clicked, this, &InterfaceRoute::on_button_LoadingMap_clicked);
    //connect(ui->button_Save, &QPushButton::clicked, this, &InterfaceRoute::on_button_Save_clicked);



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



void InterfaceRoute::loadMapFromXml(const QString& fileName) {
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }

    QXmlStreamReader reader(&file);
    QPolygonF currentPolygon;
    int passIndex = 0;

    while (!reader.atEnd() && !reader.hasError()) {
        QXmlStreamReader::TokenType token = reader.readNext();
        if (token == QXmlStreamReader::StartElement) {
            QXmlStreamAttributes attributes = reader.attributes();

            if (reader.name() == "point") {
                qreal x = attributes.value("x").toString().toDouble();
                qreal y = attributes.value("y").toString().toDouble();
                QPointF point(x, y);
                currentPolygon << point;
            } else if (reader.name() == "index") {
                if (attributes.hasAttribute("id")) {
                    passIndex = attributes.value("id").toInt();
                    indexes.push_back(passIndex);

                    // Add the polygon to the scene
                    scene->addPolygon(currentPolygon, QPen(Qt::white), QBrush(Qt::lightGray));
                    Polygons.push_back(currentPolygon);

                    // Calculate the centroid of the polygon for index positioning
                    QPointF centroid(0, 0);
                    for (const QPointF& point : currentPolygon) {
                        centroid += point;
                    }
                    centroid /= currentPolygon.size();

                    // Add index text at the centroid
                    QGraphicsTextItem* indexItem = scene->addText(QString::number(passIndex));
                    indexItem->setPos(centroid);
                    indexItem->setDefaultTextColor(Qt::blue);

                    // Clear the current polygon for the next one
                    currentPolygon.clear();
                }
            }
        }
    }

    if (reader.hasError()) {
        qDebug() << "XML parsing error";
    }
    file.close();
}

//проверка, является ли точка в полигоне
bool InterfaceRoute::searchPoint(QPointF pt){
    for(size_t i = 0; i < Polygons.size(); i++){
        if(Polygons[i].containsPoint(pt, Qt::OddEvenFill)) return true;
    }
    return false;
}



//установка границ на создание соседних точек
bool isValid(int x, int y, int n) {
    return (x >= 0 && x < n&& y >= 0 && y < n);
}

//вычисление евристической величины ( расстояния до конечной точки)
int heuristic(int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) + abs(y1 - y2);
}


int InterfaceRoute::findCost(Node* current, Node* goal) {
    if (flag == 1){
        return current->heuristic + 1;
    }
    if(current->Point.x() == goal->Point.x() && current->Point.y() == goal->Point.y()){
        flag = 0;
        return current->cost + 1;
    }
    if(flag == 0){
        return current->cost - 1;
    }
    return current->cost;
}



std::vector<Node> InterfaceRoute::getNeighbors(Node* node, Node* goal, int n) {
    std::vector<Node> neighbors;
    int x = node->Point.x();
    int y = node->Point.y();

    if (isValid(x + 1, y, n)) {
        QPointF rightNeighbor(x + 1, y);
        if (!searchPoint(rightNeighbor))
            neighbors.push_back(Node(x + 1, y, node->cost + 1, heuristic(x + 1, y, goal->Point.x(), goal->Point.y())));
    }

    if (isValid(x - 1, y, n)) {
        QPointF leftNeighbor(x - 1, y);
        if (!searchPoint(leftNeighbor))
            neighbors.push_back(Node(x - 1, y, node->cost + 1, heuristic(x - 1, y, goal->Point.x(), goal->Point.y())));
    }

    if (isValid(x, y + 1, n)) {
        QPointF downNeighbor(x, y + 1);
        if (!searchPoint(downNeighbor))
            neighbors.push_back(Node(x, y + 1, node->cost + 1, heuristic(x, y + 1, goal->Point.x(), goal->Point.y())));
    }

    if (isValid(x, y - 1, n)) {
        QPointF upNeighbor(x, y - 1);
        if (!searchPoint(upNeighbor))
            neighbors.push_back(Node(x, y - 1, node->cost + 1, heuristic(x, y - 1, goal->Point.x(), goal->Point.y())));
    }

    return neighbors;
}



//поиск среди точек на повторения (бывают попадания на одних и тех же соседей-точек)
bool searching(std::vector<Node> visited, Node current) {
    if (!visited.empty()) {
        for (Node obj : visited) {
            if (current.Point.x() == obj.Point.x() && current.Point.y() == obj.Point.y()) return true;
        }
    }
    return false;
}


//Приблизительный алгоритм A*
std::vector<Node> InterfaceRoute::aStar(Node start, Node goal, int n) {
    std::vector<Node> closedSet;
    std::vector<Node> openSet;
    std::vector<Node> visited;
    openSet.push_back(start);

    while (!openSet.empty()) {
        Node current = openSet[0];
        int currentIndex = 0;
        //Если среди новых точек есть та, у которой расстояние будет меньше рассматриваемой, то рассматриваемую возьмут эту новую точку
        for (int i = 1; i < openSet.size(); i++) {
            if (openSet[i].cost + openSet[i].heuristic < current.cost + current.heuristic) {
                current = openSet[i];
                currentIndex = i;
            }
        }

        openSet.erase(openSet.begin() + currentIndex);
        closedSet.push_back(current);


        //Когда путь будет сформирован от начала до конца, то алгоритм будет выводить итог кратчайшего пути
        if (current.Point.x() == goal.Point.x() && current.Point.y() == goal.Point.y()) {
            qDebug() << closedSet.size() << " " << openSet.size();
            std::vector<Node> path;
            path.push_back(goal);
            for (int i = closedSet.size()-2; i > 0; i--) {
                if(closedSet[i].cost <= current.cost && !searchPoint(closedSet[i].Point)){
                    path.push_back(current);
                    visited.push_back(current);

                    current = closedSet[i];
                }
            }
            path.push_back(start);
            return path;
        }
        //Проверка точек-соседей на нахождение или отсутствие в закрытом(точки конечного пути) и открытом(множество новых нерассмотренных точек) списках
        std::vector<Node> neighbors = getNeighbors(&current, &goal, n);
        for (Node neighbor : neighbors) {
            bool isClosed = false;
            for (Node closedNode : closedSet) {
                if (closedNode.Point.x() == neighbor.Point.x() && closedNode.Point.y() == neighbor.Point.y()) {
                    isClosed = true;
                    break;
                }
            }

            if (!isClosed) {
                bool isOpen = false;
                for (Node openNode : openSet) {
                    if (openNode.Point.x() == neighbor.Point.x() && openNode.Point.y() == neighbor.Point.y()) {
                        isOpen = true;
                        if (current.cost < openNode.cost) {
                            openNode.cost = current.cost;
                        }
                        break;
                    }
                }

                if (!isOpen) {
                    neighbor.cost = current.cost;
                    openSet.push_back(neighbor);
                }
            }
        }
    }

    return std::vector<Node>();
}




double InterfaceRoute::distance(QPointF* current, QPointF* neighbor) {
    return std::sqrt(std::pow(current->x() - neighbor->x(), 2) + std::pow(current->y() - neighbor->y(), 2));
}



void InterfaceRoute::on_button_LoadingMap_clicked() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Map"), "", tr("XML Files (*.xml)"));
    if (fileName.isEmpty()) {
        return;
    }
    if(!Polygons.empty()) Polygons.clear();
    scene->clear();
    StartPoint = nullptr;
    FinishPoint = nullptr;
    loadMapFromXml(fileName);
}



void InterfaceRoute::on_button_StartJourney_clicked() {
    if (StartPoint && FinishPoint) {
        // Проверяем, находятся ли стартовая и конечная точки внутри препятствий
        if (searchPoint(*StartPoint) || searchPoint(*FinishPoint)) {
            qDebug() << "Start or finish point cannot be set on an obstacle";
            return;
        }

        Node Start(StartPoint->x(), StartPoint->y(), 0, heuristic(StartPoint->x(), StartPoint->y(), FinishPoint->x(), FinishPoint->y()));
        Node Finish(FinishPoint->x(), FinishPoint->y(), 0, 0);
        if (aStar(Start, Finish, n).size() < aStar(Finish, Start, n).size()) {
            WayPoints = aStar(Start, Finish, n);
        }
        else {
            WayPoints = aStar(Finish, Start, n);
        }

        qDebug() << WayPoints.size();

        for (int i = 0; i < WayPoints.size() - 2; i++) {
            QLineF line(WayPoints[i].Point.x(), WayPoints[i].Point.y(), WayPoints[i + 1].Point.x(), WayPoints[i + 1].Point.y());
            scene->addLine(line, QPen(Qt::yellow, 2));
        }

        QGraphicsEllipseItem *startEllipse = new QGraphicsEllipseItem(StartPoint->x() - 7, StartPoint->y() - 7, 15, 15);
        startEllipse->setBrush(Qt::green);
        scene->addItem(startEllipse);

        QGraphicsEllipseItem *finishEllipse = new QGraphicsEllipseItem(FinishPoint->x() - 5, FinishPoint->y() - 7, 15, 15);
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
    WayPoints.clear();
    StartPoint = nullptr;
    FinishPoint = nullptr;
}



void InterfaceRoute::on_button_Save_clicked() {
    if (WayPoints.empty()) {
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
    for (size_t i = 0; i < WayPoints.size(); ++i) {
        Node& node = WayPoints[i];
        xmlWriter.writeStartElement("WayPoint");
        xmlWriter.writeAttribute("x", QString::number(node.Point.x()));
        xmlWriter.writeAttribute("y", QString::number(node.Point.y()));
        xmlWriter.writeAttribute("cost", QString::number(node.cost));
        xmlWriter.writeAttribute("heuristic", QString::number(node.heuristic));
        xmlWriter.writeEndElement(); // WayPoint

        if (i > 0) {
            totalDistance += distance(&WayPoints[i-1].Point, &node.Point);
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



void InterfaceRoute::mousePressEvent(QMouseEvent *event){
    if(event->button() == Qt::LeftButton){
        if(!StartPoint){
            StartPoint = new QPointF( - ui->graphicsView->x() + event->pos().x(), - ui->graphicsView->y() + event->pos().y());
            QGraphicsItem* pointItem = new QGraphicsEllipseItem(0,0,2,2);
            pointItem->setPos( - ui->graphicsView->x() + event->pos().x(), - ui->graphicsView->y() + event->pos().y());
            scene->addItem(pointItem);
            qDebug() << "Start point set";
        }
        else if(!FinishPoint){
            FinishPoint = new QPointF( - ui->graphicsView->x() + event->pos().x(), - ui->graphicsView->y() + event->pos().y());
            QGraphicsItem* pointItem = new QGraphicsEllipseItem(0,0,2,2);
            pointItem->setPos( - ui->graphicsView->x() + event->pos().x(), - ui->graphicsView->y() + event->pos().y());
            scene->addItem(pointItem);
            qDebug() << "Finish point set";
        }
        else{
            qDebug() << "Both points already set";
        }
    }
}

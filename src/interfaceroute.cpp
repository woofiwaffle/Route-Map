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

                    QGraphicsTextItem* indexItem = scene->addText(QString::number(passIndex));
                    indexItem->setPos(QLineF(Polygon[0].x(), Polygon[0].y(), Polygon[Polygon.size()/2].x(), Polygon[Polygon.size()/2].y()).center());
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

//проверка, является ли точка в полигоне
bool InterfaceRoute::searchPoint(QPointF pt){
    for(size_t i = 0; i < Polygons.size(); i++){
        if(Polygons[i].containsPoint(pt, Qt::OddEvenFill)) return true;
    }
    return false;
}


//нахождение среди соседей точек той, у которой cost меньше всех
Node InterfaceRoute::MinCost(vector<Node> Neighbors){
    Node Mincost = Neighbors[0];
    for(Node neighbor : Neighbors){
        if(neighbor.cost <= Mincost.cost && !searchPoint(neighbor.Point)){
            Mincost = neighbor;
        }
    }
    return Mincost;
}



//установка границ на создание соседних точек
bool isValid(int x, int y, int n) {
    return (x >= 0 && x < n&& y >= 0 && y < n);
}

//вычисление евристической величины ( расстояния до конечной точки)
int heuristic(int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) + abs(y1 - y2);
}


int InterfaceRoute::findCost(Node* current, Node* start, Node* goal) {
    if (flag == 1){
        return current->heuristic + 1;
    }
    if(current->Point.x() == goal->Point.x() && current->Point.y() == goal->Point.y()){
        flag = 0;
        return current->cost;
    }
    if(flag == 0){
        return current->cost - 1;
    }
    return current->cost;
}

//получение точек-соседей
std::vector<Node> InterfaceRoute::getNeighbors(Node* node, Node* start, Node* goal, int n) {
    std::vector<Node> neighbors;
    int x = node->Point.x();
    int y = node->Point.y();
    if (isValid(x + 1, y, n) && !searchPoint(QPointF(x + 1, y)))
        neighbors.push_back(Node(x + 1, y, heuristic(start->Point.x(), start->Point.y(), x + 1, y), heuristic(x + 1, y, goal->Point.x(), goal->Point.y())));

    if (isValid(x - 1, y, n) && !searchPoint(QPointF(x - 1, y)))
        neighbors.push_back(Node(x - 1, y, heuristic(start->Point.x(), start->Point.y(), x - 1, y), heuristic(x - 1, y, goal->Point.x(), goal->Point.y())));

    if (isValid(x, y + 1, n) && !searchPoint(QPointF(x, y + 1)))
        neighbors.push_back(Node(x, y+1, heuristic(start->Point.x(), start->Point.y(), x, y + 1), heuristic(x, y + 1, goal->Point.x(), goal->Point.y())));

    if (isValid(x, y - 1, n) && !searchPoint(QPointF(x, y - 1)))
        neighbors.push_back(Node(x, y - 1, heuristic(start->Point.x(), start->Point.y(), x, y - 1), heuristic(x, y - 1, goal->Point.x(), goal->Point.y())));

    if (isValid(x - 1, y - 1, n) && !searchPoint(QPointF(x - 1, y - 1)))
        neighbors.push_back(Node(x - 1, y - 1, heuristic(start->Point.x(), start->Point.y(), x - 1, y - 1), heuristic(x - 1, y - 1, goal->Point.x(), goal->Point.y())));

    if (isValid(x + 1, y + 1, n) && !searchPoint(QPointF(x + 1, y + 1)))
        neighbors.push_back(Node(x + 1, y + 1, heuristic(start->Point.x(), start->Point.y(), x + 1, y + 1), heuristic(x + 1, y + 1, goal->Point.x(), goal->Point.y())));

    if (isValid(x - 1, y + 1, n) && !searchPoint(QPointF(x - 1, y + 1)))
        neighbors.push_back(Node(x - 1, y + 1, heuristic(start->Point.x(), start->Point.y(), x - 1, y + 1), heuristic(x - 1, y + 1, goal->Point.x(), goal->Point.y())));

    if (isValid(x + 1, y - 1, n) && !searchPoint(QPointF(x + 1, y - 1)))
        neighbors.push_back(Node(x + 1, y - 1, heuristic(start->Point.x(), start->Point.y(), x + 1, y - 1), heuristic(x + 1, y - 1, goal->Point.x(), goal->Point.y())));

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
            if (openSet[i].cost + openSet[i].heuristic <= current.cost + current.heuristic) {
                current = openSet[i];
                currentIndex = i;
            }
        }

        openSet.erase(openSet.begin() + currentIndex);
        closedSet.push_back(current);


        //Когда путь будет сформирован от начала до конца, то алгоритм будет выводить итог кратчайшего пути
        if (current.Point.x() == goal.Point.x() && current.Point.y() == goal.Point.y()) {
            std::vector<Node> path;
            qDebug() << closedSet.size() << " " << openSet.size();
            while (current.Point.x() != start.Point.x() || current.Point.y() != start.Point.y()) {
                path.push_back(current);
                visited.push_back(current);

                std::vector<Node> Neighbors = getNeighbors(&current, &start, &goal, n);

                for (Node neighbor : Neighbors) {
                    if((!searchPoint(neighbor.Point) && !searching(visited, neighbor)) && (neighbor.Point.x() == MinCost(Neighbors).Point.x() && neighbor.Point.y() == MinCost(Neighbors).Point.y())){
                        current = neighbor;
                    }
                }
            }
            path.push_back(start);
            return path;
        }
        //Проверка точек-соседей на нахождение или отсутствие в закрытом(точки конечного пути) и открытом(множество новых нерассмотренных точек) списках
        std::vector<Node> neighbors = getNeighbors(&current, &start, &goal, n);
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
                        if (findCost(&current, &start, &goal) < findCost(&openNode, &start, &goal) && flag == 1) {
                            openNode.cost = findCost(&current, &start, &goal);
                        }
                        else if((findCost(&current, &start, &goal) > findCost(&openNode, &start, &goal) && flag == 0)) {
                            openNode.cost = findCost(&current, &start, &goal);
                        }
                        break;
                    }
                }

                if (!isOpen) {
                    neighbor.cost = findCost(&current, &start, &goal);
                    openSet.push_back(neighbor);
                }
            }
        }
    }

    return std::vector<Node>();
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
    if(StartPoint && FinishPoint){
        //findOptimalRoute(StartPoint, FinishPoint);
        Node Start(StartPoint->x(), StartPoint->y(), 0, heuristic(StartPoint->x(), StartPoint->y(), FinishPoint->x(), FinishPoint->y()));
        Node Finish(FinishPoint->x(), FinishPoint->y(), 0, 0);
        std::vector<Node> path = aStar(Start, Finish, n);
        qDebug() << path.size();
        for(int i = 0; i < path.size()-2; i++){
            QLineF line(path[i].Point.x(), path[i].Point.y(), path[i+1].Point.x(), path[i+1].Point.y());
            scene->addLine(line, QPen(Qt::red));
        }
    }
    else{
        qDebug() << "Start and/or finish point not set";
    }
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



/*
// Вычисление эвристической оценки (евклидово расстояние) от текущей точки до конечной точки
double InterfaceRoute::heuristic(QPointF* current, QPointF* finish) {
    return QLineF(current->x(), current->y(), finish->x(), finish->y()).length();
}

// Получение списка соседних точек для данной точки на карте
vector<QPointF*> InterfaceRoute::getNeighbors(QPointF* current) {
    vector<QPointF*> neighbors;
    int x = current->x() / 10; // Размер ячейки на карте
    int y = current->y() / 10;

    // Проверяем соседние клетки по горизонтали и вертикали
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if (dx == 0 && dy == 0) continue; // Пропускаем текущую точку

            int nx = x + dx;
            int ny = y + dy;

            if (nx >= 0 && nx < 10 && ny >= 0 && ny < 10) { // Проверяем, что соседние клетки находятся в пределах карты
                QPointF* neighbor = new QPointF(nx * 10, ny * 10);
                neighbors.push_back(neighbor);
            }
        }
    }
    // Реализуйте ваш код здесь, чтобы определить соседние точки для данной точки на карте
    return neighbors;
}

// Вычисление стоимости перемещения между двумя точками
double InterfaceRoute::distance(QPointF* current, QPointF* neighbor) {
    return QLineF(current->x(), current->y(), neighbor->x(), neighbor->y()).length();
}

// Восстановление оптимального пути, начиная с конечной точки и перемещаясь к начальной точке по указанным родителям узлов
vector<QPointF*> InterfaceRoute::reconstructPath(Node* endNode) {
    vector<QPointF*> path;
    Node* current = endNode;
    while (current != nullptr) {
        //path.push_back(current->item);
        //current = current->parent;
    }
    std::reverse(path.begin(), path.end()); // Переворачиваем путь, чтобы он начинался с начальной точки
    return path;
}
*/

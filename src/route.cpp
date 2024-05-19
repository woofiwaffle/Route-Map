#include "headers/route.h"



void Route::loadMapFromXml(const QString& fileName, QGraphicsScene* scene) {
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        return;
    }

    QXmlStreamReader reader(&file);
    QPolygonF currentPolygon;
    int passIndex = 0;

    while(!reader.atEnd() && !reader.hasError()){
        QXmlStreamReader::TokenType token = reader.readNext();
        if(token == QXmlStreamReader::StartElement){
            QXmlStreamAttributes attributes = reader.attributes();

            if(reader.name() == "point"){
                qreal x = attributes.value("x").toString().toDouble();
                qreal y = attributes.value("y").toString().toDouble();
                QPointF point(x, y);
                currentPolygon << point;
            }
            else if (reader.name() == "index"){
                if(attributes.hasAttribute("id")){
                    passIndex = attributes.value("id").toInt();
                    obstacle.indexes.push_back(passIndex);

                    // Add the polygon to the scene
                    scene->addPolygon(currentPolygon, QPen(Qt::white), QBrush(Qt::lightGray));
                    Polygons.push_back(currentPolygon);
                    obstacle.Polygons.push_back(currentPolygon);

                    // Calculate the centroid of the polygon for index positioning
                    QPointF centroid(0, 0);
                    for (const QPointF& point : currentPolygon) {
                        centroid += point;
                    }
                    centroid /= currentPolygon.size();

                    // Add index text at the centroid
                    QGraphicsTextItem* indexItem = scene->addText(QString::number(passIndex));
                    indexItem->setPos(centroid);
                    if(passIndex <= 40){
                       indexItem->setDefaultTextColor(Qt::darkGreen);  // Зелёный для значений <= 40
                    }
                    else if(passIndex <= 70){
                        indexItem->setDefaultTextColor(Qt::darkYellow); // Жёлтый для значений 41-70
                    }
                    else{
                        indexItem->setDefaultTextColor(Qt::darkRed);    // Красный для значений > 70
                    }

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
bool Route::searchPoint(QPointF pt){
    for(size_t i = 0; i < Polygons.size(); i++){
        if(Polygons[i].containsPoint(pt, Qt::OddEvenFill))
            if(obstacle.indexes[i] > 40)
                return true;
    }
    return false;
}


//установка границ на создание соседних точек
bool isValid(int x, int y, int n) {
    return (x >= 0 && x < n&& y >= 0 && y < n);
}


//вычисление евристической величины ( расстояния до конечной точки)
int Route::heuristic(int x1, int y1, int x2, int y2) {
    int dx = abs(x1 - x2);
    int dy = abs(y1 - y2);
    return 50 * sqrt(dx*dx + dy*dy);  // Алгоритм Евклида
}



int Route::findCost(Node* current, Node* goal) {
    if(flag == 1){
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


//поиск среди точек на повторения (бывают попадания на одних и тех же соседей-точек)
bool searching(std::vector<Node> visited, Node current) {
    return std::find_if(visited.begin(), visited.end(), [&](Node& obj){
        return obj.Point.x() == current.Point.x() && obj.Point.y() == current.Point.y();
    }) != visited.end();
}

// алгоритм A*
std::vector<Node> Route::aStar(Node start, Node goal, int n) {
    std::vector<Node> closedSet;
    std::vector<Node> openSet;
    std::vector<Node> visited;
    openSet.emplace_back(start);

    while (!openSet.empty()) {
        Node current = openSet[0];
        int currentIndex = 0;
        // Если среди новых точек есть та, у которой расстояние будет меньше рассматриваемой, то рассматриваемую возьмут эту новую точку
        for (int i = 1; i < openSet.size(); i++) {
            if (openSet[i].cost + openSet[i].heuristic < current.cost + current.heuristic) {
                current = openSet[i];
                currentIndex = i;
            }
        }

        openSet.erase(openSet.begin() + currentIndex);
        closedSet.push_back(current);

        // Когда путь будет сформирован от начала до конца, алгоритм выводит итог кратчайшего пути
        if (current.Point.x() == goal.Point.x() && current.Point.y() == goal.Point.y()) {
            qDebug() << closedSet.size();
            std::vector<Node> path;
            path.push_back(goal);
            for (int i = closedSet.size() - 2; i > 0; i--) {
                if (closedSet[i].cost <= current.cost && !searchPoint(closedSet[i].Point)) {
                    path.push_back(current);
                    visited.push_back(current);
                    current = closedSet[i];
                }
            }
            path.push_back(start);
            return path;
        }

        // Проверка точек-соседей
        std::vector<Node> neighbors = getNeighbors(&current, &goal, n);
        for (Node neighbor : neighbors) {
            if (!searchPoint(neighbor.Point) || obstacle.indexes[&neighbor - &neighbors[0]] <= 40) {
                bool isClosed = false;
                for (Node closedNode : closedSet) {
                    if (closedNode.Point.x() == neighbor.Point.x() && closedNode.Point.y() == neighbor.Point.y()) {
                        isClosed = true;
                        break;
                    }
                }

                if (!isClosed) {
                    bool isOpen = false;
                    for (Node& openNode : openSet) {
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
                        openSet.emplace_back(neighbor);
                    }
                }
            }
        }
    }

    return std::vector<Node>();
}



double Route::distance(QPointF* current, QPointF* neighbor) {
    return std::sqrt(std::pow(current->x() - neighbor->x(), 1.4) + std::pow(current->y() - neighbor->y(), 1.4));
}



std::vector<Node> Route::getNeighbors(Node* node, Node* goal, int n) {
    std::vector<Node> neighbors;
    int x = node->Point.x();
    int y = node->Point.y();
    static const std::vector<std::pair<int, int>> directions = {
        {1, 0}, {-1, 0}, {0, 1}, {0, -1}, // Основные направления
        {1, 1}, {-1, 1}, {1, -1}, {-1, -1} // Диагональные направления
    };

    for (const auto& dir : directions) {
        int nx = x + dir.first;
        int ny = y + dir.second;
        if (isValid(nx, ny, n)) {
            QPointF neighborPoint(nx, ny);
            if (!searchPoint(neighborPoint)) {
                double moveCost = (dir.first != 0 && dir.second != 0) ? sqrt(1.4) : 1;
                neighbors.emplace_back(Node(nx, ny, node->cost + moveCost, heuristic(nx, ny, goal->Point.x(), goal->Point.y())));
            }
        }
    }

    return neighbors;
}

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

                    // Добавляем полигон на сцену
                    scene->addPolygon(currentPolygon, QPen(Qt::white), QBrush(Qt::lightGray));
                    Polygons.push_back(currentPolygon);
                    obstacle.Polygons.push_back(currentPolygon);

                    // Вычисляем центроид многоугольника для позиционирования индекса
                    QPointF centroid(0, 0);
                    for (const QPointF& point : currentPolygon) {
                        centroid += point;
                    }
                    centroid /= currentPolygon.size();

                    // Добавляем индексный текст в центроиде
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

                    // Очищаем текущий полигон для следующего
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
bool isValid(qreal x, qreal y, int n) {
    return (x >= 0 && x < n&& y >= 0 && y < n);
}


//вычисление евристической величины ( расстояния до конечной точки)
qreal Route::heuristic(qreal x1, qreal y1, qreal x2, qreal y2) {
    int dx = std::fabs(x1 - x2);
    int dy = std::fabs(y1 - y2);
    return 50 * sqrt(dx*dx + dy*dy); // Алгоритм Евклида
}


//вычисление цены (текущей цели)
qreal Route::findCost(Node* current, Node* goal) {
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
    //использование emplace вместо push, так как не требует создания временного обьекта, чем влияет на производительность
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
        closedSet.emplace_back(current);

        // Когда путь будет сформирован от начала до конца, алгоритм выводит итог кратчайшего пути
        if (current.Point.x() == goal.Point.x() && current.Point.y() == goal.Point.y()) {
            qDebug() << closedSet.size();
            std::vector<Node> path;
            path.emplace_back(goal);
            for (int i = closedSet.size() - 2; i > 0; i--) {
                if (closedSet[i].cost <= current.cost && !searchPoint(closedSet[i].Point)) {
                    path.emplace_back(current);
                    visited.emplace_back(current);
                    current = closedSet[i];
                }
            }
            path.emplace_back(start);
            return path;
        }

        // Проверка точек-соседей
        std::vector<Node> neighbors = getNeighbors(&current, &goal, n);
        for (Node neighbor : neighbors) {
            // проверка на близость к соседу
            if (!searchPoint(neighbor.Point) || obstacle.indexes[&neighbor - &neighbors[0]] <= 40) {
                bool isClosed = false;
                // проверка на закрытость соседа
                for (Node closedNode : closedSet) {
                    if (closedNode.Point.x() == neighbor.Point.x() && closedNode.Point.y() == neighbor.Point.y()) {
                        isClosed = true;
                        break;
                    }
                }

                if (!isClosed) {
                    bool isOpen = false;
                    // проверка на открытость соседа
                    for (Node& openNode : openSet) {
                        if (openNode.Point.x() == neighbor.Point.x() && openNode.Point.y() == neighbor.Point.y()) {
                            isOpen = true;
                            // если новый путь короче то обновляем информацию
                            if (current.cost < openNode.cost) {
                                openNode.cost = current.cost + distance(&current.Point, &neighbor.Point);
                                openNode.parent = &current;
                            }
                            break;
                        }
                    }

                    // если сосед не был открыт, добавляем в список открытых узлов
                    if (!isOpen) {
                        neighbor.cost = current.cost + distance(&current.Point, &neighbor.Point);
                        neighbor.parent = &current;
                        openSet.emplace_back(neighbor);
                    }
                }
            }
        }
    }

    return std::vector<Node>();
}



double Route::distance(QPointF* current, QPointF* neighbor) {
    double dx = current->x() - neighbor->x();
    double dy = current->y() - neighbor->y();
    return std::sqrt(dx * dx + dy * dy);
}



std::vector<Node> Route::getNeighbors(Node* node, Node* goal, int n) {
    std::vector<Node> neighbors;
    int x = node->Point.x();
    int y = node->Point.y();
    static const std::vector<std::pair<int, int>> directions = {
        {1, 0}, {-1, 0}, {0, 1}, {0, -1}, // Основные направления
        {1, 1}, {-1, 1}, {1, -1}, {-1, -1} // Диагональные направления
    };
    QLineF Vector = QLineF(node->Point.x(), node->Point.y(), goal->Point.x(), goal->Point.y()).unitVector();

    if(isValid(Vector.x2(), Vector.y2(), n)){
        QPointF directNeighbor(Vector.x2(), Vector.y2());
        if (!searchPoint(directNeighbor))
            neighbors.emplace_back(Node(Vector.x2(), Vector.y2(), node->cost + Vector.length(), heuristic(Vector.x2(), Vector.y2(), goal->Point.x(), goal->Point.y())));
    }

    for (const auto& dir : directions) {
        int nx = x + dir.first;
        int ny = y + dir.second;
        // проверка является ли новая позиция внутри допустимых границ
        if (isValid(nx, ny, n)) {
            QPointF neighborPoint(nx, ny);
            if (!searchPoint(neighborPoint)) {
                // вычисление стоимости перемещения
                double moveCost = (dir.first != 0 && dir.second != 0) ? sqrt(2) : 1;
                //добавляем соседа с учетом стоимости и эвристической оценки до цели
                neighbors.emplace_back(Node(nx, ny, node->cost + moveCost, heuristic(nx, ny, goal->Point.x(), goal->Point.y())));
            }
        }
    }

    return neighbors;
}

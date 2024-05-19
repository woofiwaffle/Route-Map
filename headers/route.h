#ifndef ROUTE_H
#define ROUTE_H

#include <QGraphicsScene>
#include <QPolygonF>
#include <QPointF>
#include <vector>
#include <cmath>
#include <QFileDialog>
#include <QGraphicsTextItem>
#include <QXmlStreamReader>
#include <QDebug>

#include "obstacle.h"

/**
 * @brief Структура, представляющая узел в графе маршрута.
 */
struct Node {
    QPointF Point; /**< Координаты точки узла. */
    double cost; /**< Стоимость достижения узла. */
    int heuristic; /**< Эвристическая оценка расстояния до цели. */

    /**
     * @brief Конструктор для создания узла.
     * @param x Координата x узла.
     * @param y Координата y узла.
     * @param cost Стоимость достижения узла.
     * @param heuristic Эвристическая оценка расстояния до цели.
     */
    Node(int x, int y, double cost, int heuristic)
        : Point(x, y), cost(cost), heuristic(heuristic) {}
};

/**
 * @brief Класс для построения оптимального маршрута.
 */
class Route {
public:
    std::vector<QPolygonF> Polygons; /**< Полигоны препятствий на карте. */
    std::vector<Node> WayPoints; /**< Узлы пути маршрута. */
    QPolygonF Polygon; /**< Полигон маршрута. */
    int flag = 1; /**< Флаг для обозначения состояния. */

    Obstacle obstacle; /**< Препятствие. */

    /**
     * @brief Загружает карту из XML-файла.
     * @param fileName Путь к XML-файлу.
     * @param scene Указатель на графическую сцену.
     */
    void loadMapFromXml(const QString& fileName, QGraphicsScene* scene);

    /**
     * @brief Находит оптимальный маршрут между двумя точками на карте.
     * @param start Указатель на начальную точку маршрута.
     * @param finish Указатель на конечную точку маршрута.
     */
    void findOptimalRoute(QPointF* start, QPointF* finish);

    /**
     * @brief Вычисляет расстояние между двумя точками.
     * @param current Текущая точка.
     * @param neighbor Соседняя точка.
     * @return Расстояние между точками.
     */
    double distance(QPointF* current, QPointF* neighbor);

    /**
     * @brief Вычисляет эвристическую оценку расстояния между двумя точками.
     * @param x1 Координата x первой точки.
     * @param y1 Координата y первой точки.
     * @param x2 Координата x второй точки.
     * @param y2 Координата y второй точки.
     * @return Эвристическая оценка расстояния.
     */
    int heuristic(int x1, int y1, int x2, int y2);

    /**
     * @brief Находит стоимость прохода от текущей точки к цели.
     * @param current Указатель на текущий узел.
     * @param goal Указатель на целевой узел.
     * @return Стоимость прохода от текущей точки к цели.
     */
    int findCost(Node* current, Node* goal);

    /**
     * @brief Возвращает соседние узлы для заданного узла.
     * @param node Указатель на текущий узел.
     * @param goal Указатель на целевой узел.
     * @param n Параметр для определения соседей.
     * @return Вектор соседних узлов.
     */
    std::vector<Node> getNeighbors(Node* node, Node* goal, int n);

    /**
     * @brief Проверяет, содержит ли сцена точку.
     * @param point Точка, которую нужно проверить.
     * @return true, если точка содержится на сцене, в противном случае -
     * @return false
    */
    bool searchPoint(QPointF);

    /**
    * @brief Выполняет поиск оптимального маршрута с использованием алгоритма A*.
    * @param start Начальная точка маршрута.
    * @param goal Конечная точка маршрута.
    * @param n Параметр для определения соседей.
    * @return Вектор узлов оптимального маршрута.
    */
    std::vector<Node> aStar(Node start, Node goal, int n);
};

#endif // ROUTE_H

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
 * @brief Структура для представления узла в алгоритме A*.
 * @author woofiwaffle
 * @version 0.1
 * @date Май 2024 года
 */
struct Node {
    QPointF Point;         ///< Координаты точки
    double cost;           ///< Стоимость пути до данной точки
    int heuristic;         ///< Евристическая стоимость до конечной точки

    /**
     * @brief Конструктор узла.
     * @param x Координата x
     * @param y Координата y
     * @param cost Стоимость пути до данной точки
     * @param heuristic Евристическая стоимость до конечной точки
     */
    Node(int x, int y, double cost, int heuristic)
        : Point(x, y), cost(cost), heuristic(heuristic) {}

    /**
     * @brief Конструктор по умолчанию.
     */
    Node() : Point(0, 0), cost(0), heuristic(0) {}

    /**
     * @brief Оператор сравнения для приоритетной очереди.
     * @param other Другой узел для сравнения
     * @return true если стоимость и евристика текущего узла больше
     */
    bool operator>(const Node& other) const {
        return (cost + heuristic) > (other.cost + other.heuristic);
    }
};

/**
 * @brief Класс для представления маршрута и его расчета.
 */
class Route {
public:
    std::vector<QPolygonF> Polygons;   ///< Вектор полигонов препятствий
    std::vector<Node> WayPoints;       ///< Вектор точек маршрута
    QPolygonF Polygon;                 ///< Текущий полигон
    int flag = 1;                      ///< Флаг для внутренних нужд

    Obstacle obstacle;                 ///< Объект препятствий

    /**
     * @brief Загрузка карты из XML файла.
     * @param fileName Имя файла
     * @param scene Графическая сцена для отображения карты
     */
    void loadMapFromXml(const QString& fileName, QGraphicsScene* scene);

    /**
     * @brief Поиск оптимального маршрута от стартовой до конечной точки.
     * @param start Стартовая точка
     * @param finish Конечная точка
     */
    void findOptimalRoute(QPointF* start, QPointF* finish);

    /**
     * @brief Вычисление расстояния между двумя точками.
     * @param current Текущая точка
     * @param neighbor Соседняя точка
     * @return Расстояние между точками
     */
    double distance(QPointF* current, QPointF* neighbor);

    /**
     * @brief Вычисление евристической стоимости.
     * @param x1 Координата x первой точки
     * @param y1 Координата y первой точки
     * @param x2 Координата x второй точки
     * @param y2 Координата y второй точки
     * @return Евристическая стоимость
     */
    int heuristic(int x1, int y1, int x2, int y2);

    /**
     * @brief Вычисление стоимости пути от текущего узла до целевого.
     * @param current Текущий узел
     * @param goal Целевой узел
     * @return Стоимость пути
     */
    int findCost(Node* current, Node* goal);

    /**
     * @brief Получение соседних узлов.
     * @param node Текущий узел
     * @param goal Целевой узел
     * @param n Размер карты
     * @return Вектор соседних узлов
     */
    std::vector<Node> getNeighbors(Node* node, Node* goal, int n);

    /**
     * @brief Проверка, содержится ли точка в каком-либо полигоне.
     * @param pt Точка для проверки
     * @return true если точка содержится в полигоне
     */
    bool searchPoint(QPointF);

    /**
     * @brief Алгоритм поиска A*.
     * @param start Стартовый узел
     * @param goal Целевой узел
     * @param n Размер карты
     * @return Вектор узлов, представляющих путь
     */
    std::vector<Node> aStar(Node start, Node goal, int n);
};

#endif // ROUTE_H

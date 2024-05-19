#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <QPolygonF>
#include <QInputDialog>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QFile>
#include <QXmlStreamWriter>
#include <QMessageBox>
#include <QFileDialog>

/**
 * @brief Класс, представляющий препятствие на карте.
 */
class Obstacle {
public:
    std::vector<int> indexes; /**< Индексы препятствий. */
    std::vector<QPolygonF> Polygons; /**< Полигоны препятствий на карте. */
    QPolygonF Polygon; /**< Полигон препятствия. */

    /**
     * @brief Добавляет точку к полигону препятствия.
     * @param point Точка для добавления.
     */
    void addPointToPolygon(const QPointF &point);

    /**
     * @brief Завершает формирование полигона препятствия и добавляет его на сцену.
     * @param scene Указатель на графическую сцену.
     */
    void finalizePolygon(QGraphicsScene *scene);

    /**
     * @brief Очищает все данные о препятствиях.
     */
    void clearAll();

    /**
     * @brief Сохраняет препятствия в XML-файл.
     */
    void saveToXml();
};

#endif // OBSTACLE_H

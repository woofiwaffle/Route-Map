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
 * @brief Класс представления препятсвия
 * @author woofiwaffle
 * @version 0.1
 * @date Май 2024 года
 *
 * Класс Obstacle представляет собой препятствие на карте.
 * Он позволяет добавлять точки для определения полигона препятствия,
 * завершать определение полигона, очищать все точки и сохранять
 * препятствие в XML-файл.
 */
class Obstacle {
public:
    std::vector<int> indexes; /**< Индексы точек в полигоне препятствия */
    std::vector<QPolygonF> Polygons; /**< Список полигонов, представляющих препятствия */
    QPolygonF Polygon; /**< Текущий полигон, который определяется */

    /**
     * @brief Добавляет точку к текущему полигону.
     * @param point Точка для добавления к полигону.
     */
    void addPointToPolygon(const QPointF &point);

    /**
     * @brief Завершает определение текущего полигона.
     * @param scene Графическая сцена, на которой отрисовывается полигон.
     */
    void finalizePolygon(QGraphicsScene *scene);

    /**
     * @brief Очищает все точки и полигоны.
     */
    void clearAll();

    /**
     * @brief Сохраняет препятствие в XML-файл.
     */
    void saveToXml();
};

#endif // OBSTACLE_H

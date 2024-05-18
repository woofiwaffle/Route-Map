#ifndef INTERFACEMAP_H
#define INTERFACEMAP_H

#include <QWidget>
#include <QMouseEvent>
#include "obstacle.h"

namespace Ui { class InterfaceMap; }

/**
 * @brief Интерфейс для отображения карты.
 * @author woofiwaffle
 * @version 0.1
 * @date Май 2024 года
 *
 * Этот класс представляет собой интерфейс для отображения карты и управления ею.
 */
class InterfaceMap : public QWidget {
    Q_OBJECT

public:
    /**
     * @brief Конструктор класса InterfaceMap.
     * @param parent Родительский объект (по умолчанию nullptr).
     */
    explicit InterfaceMap(QWidget *parent = nullptr);

    /**
     * @brief Деструктор класса InterfaceMap.
     */
    ~InterfaceMap();

    /**
     * @brief Обработчик нажатия кнопки мыши.
     * @param event Событие мыши.
     */
    void mousePressEvent(QMouseEvent *event);

private:
    Ui::InterfaceMap *ui; /**< Интерфейс окна карты */
    QGraphicsScene *scene; /**< Сцена для отображения графики */
    Obstacle obstacle; /**< Объект препятствия на карте */

private slots:
    /**
     * @brief Возврат к основному окну.
     */
    void backToMain();

    /**
     * @brief Обработчик нажатия кнопки "Очистить карту".
     */
    void on_button_ClearMap_clicked();

    /**
     * @brief Обработчик нажатия кнопки "Сохранить".
     */
    void on_button_Save_clicked();
};

#endif // INTERFACEMAP_H

#ifndef INTERFACEMAP_H
#define INTERFACEMAP_H

#include <QWidget>
#include <QMouseEvent>
#include "obstacle.h"

namespace Ui { class InterfaceMap; }

/**
 * @brief Класс, представляющий интерфейс для работы с картой.
 */
class InterfaceMap : public QWidget {
    Q_OBJECT

public:
    /**
     * @brief Конструктор класса InterfaceMap.
     * @param parent Родительский объект.
     */
    explicit InterfaceMap(QWidget *parent = nullptr);

    /**
     * @brief Деструктор класса InterfaceMap.
     */
    ~InterfaceMap();

    /**
     * @brief Обрабатывает событие нажатия мыши.
     * @param event Событие нажатия мыши.
     */
    void mousePressEvent(QMouseEvent *event);

private:
    Ui::InterfaceMap *ui; /**< Указатель на объект интерфейса InterfaceMap. */
    QGraphicsScene *scene; /**< Указатель на графическую сцену. */
    Obstacle obstacle; /**< Объект препятствия. */

private slots:
    /**
     * @brief Возвращает пользователя в главное окно.
     */
    void backToMain();

    /**
     * @brief Обрабатывает нажатие кнопки "Clear Map".
     */
    void on_button_ClearMap_clicked();

    /**
     * @brief Обрабатывает нажатие кнопки "Save".
     */
    void on_button_Save_clicked();
};

#endif // INTERFACEMAP_H

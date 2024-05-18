#ifndef INTERFACEROUTE_H
#define INTERFACEROUTE_H

#include <QWidget>
#include <QThread>
#include "route.h"

namespace Ui { class InterfaceRoute; }

/**
 * @brief Интерфейс для маршрута.
 * @author woofiwaffle
 * @version 0.1
 * @date Май 2024 года
 *
 * Этот класс представляет собой интерфейс для работы с маршрутом.
 * Он содержит методы для управления маршрутом, загрузки карты, начала путешествия,
 * сохранения данных и очистки маршрута.
 */
class InterfaceRoute : public QWidget {
    Q_OBJECT

public:
    /**
     * @brief Конструктор класса InterfaceRoute.
     * @param parent Родительский объект (по умолчанию nullptr).
     */
    InterfaceRoute(QWidget *parent = nullptr);

    /**
     * @brief Деструктор класса InterfaceRoute.
     */
    ~InterfaceRoute();

private:
    Ui::InterfaceRoute *ui; /**< Интерфейс окна маршрута */
    QGraphicsScene *scene; /**< Сцена для отображения графики */
    QPointF* StartPoint = nullptr; /**< Точка начала маршрута */
    QPointF* FinishPoint = nullptr; /**< Точка окончания маршрута */
    Route route; /**< Объект маршрута */

    int n = 1200; /**< Размерность */

private slots:
    /**
     * @brief Обработчик нажатия кнопки мыши.
     * @param event Событие мыши.
     */
    void mousePressEvent(QMouseEvent *);

    /**
     * @brief Возврат к основному окну.
     */
    void backToMain();

    /**
     * @brief Обработчик нажатия кнопки "Загрузить карту".
     */
    void on_button_LoadingMap_clicked();

    /**
     * @brief Обработчик нажатия кнопки "Начать путешествие".
     */
    void on_button_StartJourney_clicked();

    /**
     * @brief Обработчик нажатия кнопки "Сохранить".
     */
    void on_button_Save_clicked();

    /**
     * @brief Обработчик нажатия кнопки "Очистить маршрут".
     */
    void on_button_ClearWay_clicked();
};

#endif // INTERFACEROUTE_H


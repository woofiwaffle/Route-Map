#ifndef INTERFACEROUTE_H
#define INTERFACEROUTE_H

#include <QWidget>
#include <QThread>
#include "route.h"

namespace Ui { class InterfaceRoute; }

/**
 * @brief Класс, представляющий интерфейс для построения маршрутов.
 */
class InterfaceRoute : public QWidget {
    Q_OBJECT

public:
    /**
     * @brief Конструктор класса InterfaceRoute.
     * @param parent Родительский объект.
     */
    InterfaceRoute(QWidget *parent = nullptr);

    /**
     * @brief Деструктор класса InterfaceRoute.
     */
    ~InterfaceRoute();

private:
    Ui::InterfaceRoute *ui; /**< Указатель на объект интерфейса InterfaceRoute. */
    QGraphicsScene *scene; /**< Указатель на графическую сцену. */
    QPointF* StartPoint = nullptr; /**< Указатель на начальную точку маршрута. */
    QPointF* FinishPoint = nullptr; /**< Указатель на конечную точку маршрута. */
    Route route; /**< Объект маршрута. */
    int n = 1200; /**< Параметр для определения соседей. */

private slots:
    /**
     * @brief Обрабатывает событие нажатия мыши.
     * @param event Событие нажатия мыши.
     */
    void mousePressEvent(QMouseEvent *);

    /**
     * @brief Возвращает пользователя в главное окно.
     */
    void backToMain();

    /**
     * @brief Обрабатывает нажатие кнопки "Loading Map".
     */
    void on_button_LoadingMap_clicked();

    /**
     * @brief Обрабатывает нажатие кнопки "Start Journey".
     */
    void on_button_StartJourney_clicked();

    /**
     * @brief Обрабатывает нажатие кнопки "Save".
     */
    void on_button_Save_clicked();

    /**
     * @brief Обрабатывает нажатие кнопки "Clear Way".
     */
    void on_button_ClearWay_clicked();
};

#endif // INTERFACEROUTE_H


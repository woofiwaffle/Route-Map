#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>

#include "headers/interfaceroute.h"
#include "headers/interfacemap.h"

namespace Ui { class MainWindow; }

/**
 * @brief Основное окно приложения.
 * @author woofiwaffle
 * @version 0.1
 * @date Май 2024 года
 *
 * Этот класс представляет собой основное окно приложения.
 * Он содержит две кнопки для открытия окон карты и маршрута.
 */
class MainWindow : public QWidget {
    Q_OBJECT

public:
    /**
     * @brief Конструктор класса MainWindow.
     * @param parent Родительский объект (по умолчанию nullptr).
     */
    MainWindow(QWidget *parent = nullptr);

    /**
     * @brief Деструктор класса MainWindow.
     */
    ~MainWindow();

private:
    Ui::MainWindow *ui; /**< Интерфейс основного окна */

private slots:
    /**
     * @brief Открытие окна карты.
     */
    void openMapWindow();

    /**
     * @brief Открытие окна маршрута.
     */
    void openRouteWindow();
};

#endif // MAINWINDOW_H

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>

#include "headers/interfaceroute.h"
#include "headers/interfacemap.h"

namespace Ui { class MainWindow; }

/**
 * @brief Класс, представляющий главное окно приложения.
 */
class MainWindow : public QWidget {
    Q_OBJECT

public:
    /**
     * @brief Конструктор класса MainWindow.
     * @param parent Родительский объект.
     */
    MainWindow(QWidget *parent = nullptr);

    /**
     * @brief Деструктор класса MainWindow.
     */
    ~MainWindow();

private:
    Ui::MainWindow *ui; /**< Указатель на объект интерфейса главного окна. */

private slots:
    /**
     * @brief Открывает окно для работы с картой.
     */
    void openMapWindow();

    /**
     * @brief Открывает окно для построения маршрута.
     */
    void openRouteWindow();
};

#endif // MAINWINDOW_H

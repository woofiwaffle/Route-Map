#ifndef TINTERFACE_H
#define TINTERFACE_H

#include "parsingmap.h"

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QFileDialog>

/**
 * @brief The Interface class
 */
class TInterface : public QWidget {
    Q_OBJECT

    QPushButton* updateMapButton;
    QPushButton* addObstacleButton;
    QPushButton* editObstacleButton;
    QPushButton* removeObstacleButton;
    QPushButton* setStartButton;
    QPushButton* setFinishButton;


public:
    TInterface(QWidget *parent = nullptr);
    ~TInterface();

private slots:
    void updateMap();
    void addObstacle();
    void editObstacle();
    void removeObstacle();
    void setStartPoint();
    void setFinishPoint();
    void resultAndSave();
};

#endif // TINTERFACE_H

#ifndef TPARSINGMAP_H
#define TPARSINGMAP_H

#include "map.h"

#include <iostream>
#include <fstream>
#include <QWidget>
#include <QFileDialog>
#include <QMessageBox>
#include <QXmlStreamReader>

using namespace std;



/**
 * @brief The TParsing class
 */
class TParsing : public QWidget {
    Q_OBJECT

public:
    TParsing(QWidget* parent = nullptr);

    //TMap* map;

    string getPath();
    short parsingFile(const string&);
    void error(const short&);
};

#endif // TPARSINGMAP_H

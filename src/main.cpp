#include "headers/interface.h"
#include "headers/parsingmap.h"
#include <QApplication>

int main(int argc, char *argv[]){
    QApplication a(argc, argv);

    TParsing parsing;
    //Interface w(&parsing);   // закомментировал т.к. не запустится экран редактирования
    TInterface w;
    w.show();


    return a.exec();
}


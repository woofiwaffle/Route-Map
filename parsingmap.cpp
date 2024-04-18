#include "parsingmap.h"

TParsing::TParsing(QWidget* parent) : QWidget(parent){
    short response = 0;
    while(response != 1 && response != -1){
        string path = getPath();
        map = new TMap();
        response = parsingFile(path);
        if(response != 1){
            error(response);
        }
    }
}




string TParsing::getPath(){ // получение пути
    QFile* openFile = new QFile(QFileDialog::getOpenFileName(nullptr, tr("Open input file"), "Load Map", tr("XML (*.xml)")));
    return openFile->fileName().toStdString();
}



short TParsing::parsingFile(const string& path){ // парсинг файла
    QFile filename(QString::fromStdString(path));
    if(!filename.open(QIODevice::ReadOnly)){
        return -1;
    }

    //QXmlStreamReader xmlReader(&filename);

    return 1;
}



void TParsing::error(const short& response) {  // обработка ошибок
    if(response == -1){
        QMessageBox::information(nullptr, "Cancelled", "File wasn't selected");
    }
    else if(response == -2){
        QMessageBox::critical(nullptr, "Error №2", "Invalid Input error");
    }
    else if(response == -3){
        QMessageBox::critical(nullptr, "Error №3", "Invalid Size error");
    }
    else if(response == -4){
        QMessageBox::critical(nullptr, "Error №4", "Non-Zero Diagonals error");
    }
    else{
        QMessageBox::critical(nullptr, "Error №5", "Unknown error");
    }
}

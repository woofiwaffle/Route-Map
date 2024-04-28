#include "headers/interface.h"
#include "headers/mainwindow.h"
#include "ui_interface.h"

TInterface::TInterface(QWidget *parent) : QWidget(parent), ui(new Ui::TInterface) {
    ui->setupUi(this);
    setWindowTitle("Map Editor");
    connect(ui->button_Back, &QPushButton::clicked, this, &TInterface::backToMain);

    scene = new QGraphicsScene(this);   // Инициализируем графическую сцену
    scene->setItemIndexMethod(QGraphicsScene::NoIndex); // настраиваем индексацию элементов

    ui->graphicsView->resize(600,600);  // Устанавливаем размер graphicsView
    ui->graphicsView->setScene(scene);  // Устанавливаем графическую сцену в graphicsView
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);    // Настраиваем рендер
    ui->graphicsView->setCacheMode(QGraphicsView::CacheBackground); // Кэш фона
    ui->graphicsView->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);


    scene->setSceneRect(100,100,700,700); // Устанавливаем размер сцены



}



TInterface::~TInterface() {
    delete ui;
}



void TInterface::backToMain() {
    MainWindow *mainWindow = new MainWindow();
    this->close();
    mainWindow->show();
}

/*void TInterface::MousePressEvent(QMouseEvent* event){
    if (event->button() == Qt::LeftButton) {
        auto point = new QGraphicsEllipseItem(QRectF(-10, -10, 10, 10));
        point->setPos(event->scenePosition());
        scene->addItem(point);
    }
}
*/

void TInterface::on_button_Back_4_clicked()
{
    MoveItem *item = new MoveItem();        // Создаём графический элемента
    item->setPos(350, 350);
    scene->addItem(item);   // Добавляем элемент на графическую сцену
}


#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "entity.h"
#include "gamemap.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
//    this->setStyleSheet("background-color: black");

    GameMap* map = new GameMap(this);
    QLayout* layout = new QHBoxLayout(this);

//    layout->addWidget(map);
//    setLayout(layout);
//    map->move(600, 600);
    map->show();
//    Entity *first = new Entity(this);
//    first->show();
//    first->move(100, 100);

}

MainWindow::~MainWindow()
{
    delete ui;
}

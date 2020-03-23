#include <QGridLayout>
#include <QDebug>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "entity.h"
#include "gamemap.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    /* TODOs:
     * 1. Create a vector with the coordinates of infected cells to go only through them when make updates.
     * 2. Separate the infection possibility between close neighbors and random cells from the map.
     *
     *
     */


    ui->setupUi(this);
//    this->setStyleSheet("background-color: black");

//    GameMap* map = new GameMap(this);

    ui->field->setLayout(new QGridLayout);
    ui->field->layout()->addWidget(&m_Game);


    connect(ui->lineEdit_incubationPeriod, &QLineEdit::textChanged, this, &MainWindow::updateInitialParameters);
    connect(ui->lineEdit_illnessPeriod, &QLineEdit::textChanged, this, &MainWindow::updateInitialParameters);
    connect(ui->lineEdit_infectionRate, &QLineEdit::textChanged, this, &MainWindow::updateInitialParameters);
    connect(ui->lineEdit_deathRate, &QLineEdit::textChanged, this, &MainWindow::updateInitialParameters);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateOutpus()
{
    ui->label_indicator_numberOfInfections->setText(QString::number(m_numberOfInfected));
}

void MainWindow::updateInitialParameters()
{
    m_incubationPeriod = ui->lineEdit_incubationPeriod->text().toDouble();
    m_illnessPeriod = ui->lineEdit_illnessPeriod->text().toDouble();
    m_infectionRate = ui->lineEdit_infectionRate->text().toDouble();
    m_deathRate = ui->lineEdit_deathRate->text().toDouble();

    m_probabilityToInfect = (m_incubationPeriod + m_illnessPeriod) / m_infectionRate;

    qDebug() << m_incubationPeriod << ',' << m_illnessPeriod << ',' << m_infectionRate << ',' << m_deathRate << ',' << m_probabilityToInfect;
}

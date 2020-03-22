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
    m_incubationPeriod = ui->lineEdit_incubationPeriod->text().toInt();
    m_illnessPeriod = ui->lineEdit_illnessPeriod->text().toInt();
    m_infectionRate = ui->lineEdit_infectionRate->text().toInt();
    m_deathRate = ui->lineEdit_deathRate->text().toDouble();

    qDebug() << m_incubationPeriod << ',' << m_illnessPeriod << ',' << m_infectionRate << ',' << m_deathRate;
}

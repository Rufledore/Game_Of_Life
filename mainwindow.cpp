#include <QGridLayout>
#include <QDebug>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "entity.h"
#include "gamemap.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_chart(new QtCharts::QChart),
    infectionSeries(new QtCharts::QLineSeries),
    yAxis(new QtCharts::QValueAxis),
    xAxis(new QtCharts::QValueAxis),
    populationMap(new PopulationMap(this))
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

//    ui->field->setLayout(new QGridLayout);
//    ui->field->layout()->addWidget(&m_Game);

    setUpChart();

    ui->graphicsView_map->setScene(populationMap.data());

    connect(ui->lineEdit_incubationPeriod, &QLineEdit::textChanged, this, &MainWindow::updateInitialParameters);
    connect(ui->lineEdit_illnessPeriod, &QLineEdit::textChanged, this, &MainWindow::updateInitialParameters);
    connect(ui->lineEdit_infectionRate, &QLineEdit::textChanged, this, &MainWindow::updateInitialParameters);
    connect(ui->lineEdit_deathRate, &QLineEdit::textChanged, this, &MainWindow::updateInitialParameters);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setUpChart()
{
    infectionSeries->append(0, 2);
    infectionSeries->append(2, 4);
    infectionSeries->append(3, 3);
    infectionSeries->append(4, 5);
    m_numberOfDays = 4;
    m_numberOfInfected = 5;

    m_chart->legend()->hide();
    m_chart->addSeries(infectionSeries.get());

    QPen pen = infectionSeries->pen();
    pen.setColor(QColor(255, 0 , 0));
    infectionSeries->setPen(pen);

//    m_chart->createDefaultAxes();
    m_chart->setTitle(QString::fromStdString("Infections"));
    m_chart->setPlotAreaBackgroundBrush(QBrush(Qt::white));
    m_chart->setPlotAreaBackgroundVisible(true);
//    m_chart->layout()->setContentsMargins(0, 0, 0, 0);
    m_chart->setBackgroundRoundness(0);
    m_chart->setMargins(QMargins(0, 0, 0, 0));

    // Set x and y axis
    xAxis->setTitleText("Time [days]");
    xAxis->setRange(0, static_cast<int>(m_numberOfDays * 1.3));
    yAxis->setTitleText("Number of infected");
    yAxis->setRange(0, static_cast<int>(m_numberOfInfected * 1.3));

//    m_chart->axes(Qt::Horizontal, infectionSeries.data()).append(xAxis.data());
//    m_chart->axes(Qt::Vertical, infectionSeries.data()).append(yAxis.data());

    m_chart->addAxis(xAxis.data(), Qt::AlignBottom);
    m_chart->addAxis(yAxis.data(), Qt::AlignLeft);

//    infectionSeries->attachAxis(m_chart->axes().first());
//    infectionSeries->attachAxis(m_chart->axes().last());
    infectionSeries->attachAxis(xAxis.data());
    infectionSeries->attachAxis(yAxis.data());
    ui->widget_chart->setRenderHint(QPainter::Antialiasing);
    ui->widget_chart->setChart(m_chart.data());


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

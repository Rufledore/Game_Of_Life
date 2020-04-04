#include <QGridLayout>
#include <QDebug>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "entity.h"
#include "populationmap.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_chart(new QtCharts::QChart),
    infectionSeries(new QtCharts::QLineSeries),
    yAxis(new QtCharts::QValueAxis),
    xAxis(new QtCharts::QValueAxis),
    populationMap(new PopulationMap(this, m_numberOfCellsPerRow)),
    runner(new SimulationRunner)
{
    /* TODOs:
     * 2. Separate the infection possibility between close neighbors and random cells from the map.
     *
     *
     */


    ui->setupUi(this);

    setUpChart();

    ui->graphicsView_map->setScene(populationMap.data());

    connect(ui->lineEdit_incubationPeriodMin, &QLineEdit::textChanged, this, &MainWindow::updateInputParameters);
    connect(ui->lineEdit_incubationPeriodMax, &QLineEdit::textChanged, this, &MainWindow::updateInputParameters);
    connect(ui->lineEdit_illnessPeriodMin,    &QLineEdit::textChanged, this, &MainWindow::updateInputParameters);
    connect(ui->lineEdit_illnessPeriodMax,    &QLineEdit::textChanged, this, &MainWindow::updateInputParameters);
    connect(ui->lineEdit_infectionRateMin,    &QLineEdit::textChanged, this, &MainWindow::updateInputParameters);
    connect(ui->lineEdit_infectionRateMax,    &QLineEdit::textChanged, this, &MainWindow::updateInputParameters);
    connect(ui->lineEdit_deathRateMin,        &QLineEdit::textChanged, this, &MainWindow::updateInputParameters);
    connect(ui->lineEdit_deathRateMax,        &QLineEdit::textChanged, this, &MainWindow::updateInputParameters);

    connect(populationMap.data(), &PopulationMap::clicked, runner.data(), &SimulationRunner::changeClickedPersonState);
    connect(runner.data(), &SimulationRunner::populationStatusUpdated, populationMap.data(), &PopulationMap::updatePopulationStatus);
    connect(runner.data(), &SimulationRunner::updatedOutputParameters, this, &MainWindow::updateOutputParametersOnGUI);


    runner->updateMap();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setUpChart()
{
//    infectionSeries->append(0, 2);
//    infectionSeries->append(2, 4);
//    infectionSeries->append(3, 3);
//    infectionSeries->append(4, 5);
//    m_numberOfDays = 4;
//    m_numberOfInfected = 5;
    OutputParameters parameters = runner->getOutputParameters();

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
    xAxis->setRange(0, static_cast<int>(parameters.numberOfDays * 1.3));
    yAxis->setTitleText("Number of infected");
    yAxis->setRange(0, static_cast<int>(parameters.numberOfInfections * 1.3));

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

void MainWindow::updateInputParameters()
{
    InputPerameters inputParameters;
    inputParameters.incubationPeriodMin = ui->lineEdit_incubationPeriodMin->text().toDouble();
    inputParameters.incubationPeriodMax = ui->lineEdit_incubationPeriodMax->text().toDouble();
    inputParameters.illnessPeriodMin = ui->lineEdit_illnessPeriodMin->text().toDouble();
    inputParameters.illnessPeriodMax = ui->lineEdit_illnessPeriodMax->text().toDouble();
    inputParameters.infectionRateMin = ui->lineEdit_infectionRateMin->text().toDouble();
    inputParameters.infectionRateMax = ui->lineEdit_infectionRateMax->text().toDouble();
    inputParameters.deathRateMin = ui->lineEdit_deathRateMin->text().toDouble();
    inputParameters.deathRateMax = ui->lineEdit_deathRateMax->text().toDouble();

    runner->UpdateInputParameters(&inputParameters);

    // persons property - m_probabilityToInfect = (m_incubationPeriod + m_illnessPeriod) / m_infectionRate;

}

void MainWindow::updateOutputParametersOnGUI(const OutputParameters *outputParameters)
{
    ui->label_indicator_numberOfInfections->setNum(outputParameters->numberOfInfections);
    ui->label_indicator_numberOfDeaths->setNum(outputParameters->numberOfDeaths);
    ui->label_indicator_numberOfDays->setNum(outputParameters->numberOfDays);
}

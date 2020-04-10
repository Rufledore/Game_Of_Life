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
    populationMap(new PopulationMap(this)),
    runner(new SimulationCore)
{
    /* TODOs:
     * 2. Separate the infection possibility between close neighbors and random cells from the map.
     *
     *
     */


    ui->setupUi(this);

    setUpChart();

    ui->graphicsView_map->setScene(populationMap.data());

    connect(ui->lineEdit_incubationPeriodMean, &QLineEdit::textChanged, this, &MainWindow::updateInputParameters);
    connect(ui->lineEdit_incubationPeriodSigma, &QLineEdit::textChanged, this, &MainWindow::updateInputParameters);
    connect(ui->lineEdit_severeIllnessPeriodMean,    &QLineEdit::textChanged, this, &MainWindow::updateInputParameters);
    connect(ui->lineEdit_severeIllnessPeriodSigma,    &QLineEdit::textChanged, this, &MainWindow::updateInputParameters);
    connect(ui->lineEdit_MildIllnessPeriodMean,    &QLineEdit::textChanged, this, &MainWindow::updateInputParameters);
    connect(ui->lineEdit_MildIllnessPeriodSigma,    &QLineEdit::textChanged, this, &MainWindow::updateInputParameters);
    connect(ui->lineEdit_percentSevereCases,    &QLineEdit::textChanged, this, &MainWindow::updateInputParameters);
    connect(ui->lineEdit_transmissionRateMin,    &QLineEdit::textChanged, this, &MainWindow::updateInputParameters);
    connect(ui->lineEdit_transmissionRateMax,    &QLineEdit::textChanged, this, &MainWindow::updateInputParameters);
    connect(ui->lineEdit_deathRateMin,        &QLineEdit::textChanged, this, &MainWindow::updateInputParameters);
    connect(ui->lineEdit_deathRateMax,        &QLineEdit::textChanged, this, &MainWindow::updateInputParameters);

    connect(populationMap.data(), &PopulationMap::clicked, runner.data(), &SimulationCore::changeClickedPersonState);
    connect(runner.data(), &SimulationCore::populationStatusUpdated, populationMap.data(), &PopulationMap::updatePopulationStatus);
    connect(runner.data(), &SimulationCore::updatedOutputParameters, this, &MainWindow::updateOutputParametersOnGUI);
    connect(ui->pushButton_Start, &QPushButton::clicked, runner.data(), &SimulationCore::update);

    runner->updateMap();

    // Set default values
    ui->lineEdit_deathRateMin->setText("3.3");
    ui->lineEdit_deathRateMax->setText("3.4");
    ui->lineEdit_percentSevereCases->setText("19.1");
    ui->lineEdit_transmissionRateMin->setText("1.5");
    ui->lineEdit_transmissionRateMax->setText("3.5");
    ui->lineEdit_incubationPeriodMean->setText("5.2");
    ui->lineEdit_incubationPeriodSigma->setText("1.45");
    ui->lineEdit_MildIllnessPeriodMean->setText("14");
    ui->lineEdit_MildIllnessPeriodSigma->setText("5");
    ui->lineEdit_severeIllnessPeriodMean->setText("32");
    ui->lineEdit_severeIllnessPeriodSigma->setText("7");
/*
    ui->lineEdit_deathRateMin->textChanged();
    ui->lineEdit_deathRateMax;
    ui->lineEdit_percentSevereCases;
    ui->lineEdit_transmissionRateMin;
    ui->lineEdit_transmissionRateMax;
    ui->lineEdit_incubationPeriodMean;
    ui->lineEdit_incubationPeriodSigma;
    ui->lineEdit_MildIllnessPeriodMean;
    ui->lineEdit_MildIllnessPeriodSigma;
    ui->lineEdit_severeIllnessPeriodMean;
    ui->lineEdit_severeIllnessPeriodSigma;

*/
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
    inputParameters.incubationPeriodMean = ui->lineEdit_incubationPeriodMean->text().toDouble();
    inputParameters.incubationPeriodSigma = ui->lineEdit_incubationPeriodSigma->text().toDouble();
    inputParameters.mildSymptomsPeriodMean = ui->lineEdit_MildIllnessPeriodMean->text().toDouble();
    inputParameters.mildSymptomsPeriodSigma = ui->lineEdit_MildIllnessPeriodSigma->text().toDouble();
    inputParameters.severeSymptomsPeriodMean = ui->lineEdit_severeIllnessPeriodMean->text().toDouble();
    inputParameters.severeSymptomsPeriodSigma = ui->lineEdit_severeIllnessPeriodSigma->text().toDouble();
    inputParameters.transmitionRateMin = ui->lineEdit_transmissionRateMin->text().toDouble();
    inputParameters.transmisionRateMax = ui->lineEdit_transmissionRateMax->text().toDouble();
    inputParameters.deathRateMin = ui->lineEdit_deathRateMin->text().toDouble();
    inputParameters.deathRateMax = ui->lineEdit_deathRateMax->text().toDouble();
    inputParameters.persentSevereCases = ui->lineEdit_percentSevereCases->text().toDouble();

    runner->UpdateInputParameters(&inputParameters);

    // persons property - m_probabilityToInfect = (m_incubationPeriod + m_illnessPeriod) / m_infectionRate;

}

void MainWindow::updateOutputParametersOnGUI(const OutputParameters *outputParameters)
{
    ui->label_indicator_numberOfInfections->setNum(outputParameters->numberOfInfections);
    ui->label_indicator_numberOfDeaths->setNum(outputParameters->numberOfDeaths);
    ui->label_indicator_numberOfDays->setNum(outputParameters->numberOfDays);
    ui->label_indicator_numberOfMildCases->setNum(outputParameters->numberOfMildSymptoms);
    ui->label_indicator_numberOfSevereCases->setNum(outputParameters->numberOfSevereSymptoms);

    static int maxNumOfInfections = 0;
    infectionSeries->append(outputParameters->numberOfDays, outputParameters->numberOfInfections);

    xAxis->setMax(outputParameters->numberOfDays + 0.1*outputParameters->numberOfDays);

    if (outputParameters->numberOfInfections > maxNumOfInfections)
        maxNumOfInfections = outputParameters->numberOfInfections;
    yAxis->setMax(maxNumOfInfections + 0.1*maxNumOfInfections);
}

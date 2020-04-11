#include <QGridLayout>
#include <QDoubleSpinBox>
#include <QDebug>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "entity.h"
#include "populationmap.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_chartCurrentInfections(new QtCharts::QChart),
    currentInfectionSeries(new QtCharts::QLineSeries),
    yAxisInfections(new QtCharts::QValueAxis),
    xAxisDays(new QtCharts::QValueAxis),
    populationMap(new PopulationMap(this)),
    simulation(new SimulationCore)
{
    /* TODOs:
     * 2. Separate the infection possibility between close neighbors and random cells from the map.
     *
     *
     */


    ui->setupUi(this);

    setUpChart();

    ui->graphicsView_map->setScene(populationMap.data());
    connect(ui->doubleSpinBox_deathRateMax, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MainWindow::updateInputParameters);
    connect(ui->doubleSpinBox_incubationPeriodMean, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MainWindow::updateInputParameters);
    connect(ui->doubleSpinBox_incubationPeriodSigma, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MainWindow::updateInputParameters);
    connect(ui->doubleSpinBox_severeIllnessPeriodMean, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MainWindow::updateInputParameters);
    connect(ui->doubleSpinBox_severeIllnessPeriodSigma, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MainWindow::updateInputParameters);
    connect(ui->doubleSpinBox_mildIllnessPeriodMean, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MainWindow::updateInputParameters);
    connect(ui->doubleSpinBox_mildIllnessPeriodSigma, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MainWindow::updateInputParameters);
    connect(ui->doubleSpinBox_percentSevereCases, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MainWindow::updateInputParameters);
    connect(ui->doubleSpinBox_transmissionRateMin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MainWindow::updateInputParameters);
    connect(ui->doubleSpinBox_transmissionRateMax, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MainWindow::updateInputParameters);
    connect(ui->doubleSpinBox_deathRateMin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MainWindow::updateInputParameters);
    connect(ui->doubleSpinBox_deathRateMax, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MainWindow::updateInputParameters);

    connect(populationMap.data(), &PopulationMap::clicked, simulation.data(), &SimulationCore::changeClickedPersonState);
    connect(simulation.data(), &SimulationCore::populationStatusUpdated, populationMap.data(), &PopulationMap::updatePopulationStatus);
    connect(simulation.data(), &SimulationCore::updatedOutputParameters, this, &MainWindow::updateOutputParametersOnGUI);

    connect(ui->pushButton_calculateNextDay, &QPushButton::clicked, simulation.data(), &SimulationCore::update);
    connect(ui->pushButton_reset, &QPushButton::clicked, this, &MainWindow::reset);

    simulation->updateMap();

    // Set default values
    ui->doubleSpinBox_deathRateMin->setValue(3.3);
    ui->doubleSpinBox_deathRateMax->setValue(3.4);
    ui->doubleSpinBox_percentSevereCases->setValue(19.1);
    ui->doubleSpinBox_transmissionRateMin->setValue(1.5);
    ui->doubleSpinBox_transmissionRateMax->setValue(3.5);
    ui->doubleSpinBox_incubationPeriodMean->setValue(5.2);
    ui->doubleSpinBox_incubationPeriodSigma->setValue(1.45);
    ui->doubleSpinBox_mildIllnessPeriodMean->setValue(14);
    ui->doubleSpinBox_mildIllnessPeriodSigma->setValue(5);
    ui->doubleSpinBox_severeIllnessPeriodMean->setValue(32);
    ui->doubleSpinBox_severeIllnessPeriodSigma->setValue(7);
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
    OutputParameters parameters = simulation->getOutputParameters();

    m_chartCurrentInfections->legend()->hide();
    m_chartCurrentInfections->addSeries(currentInfectionSeries.get());

    QPen pen = currentInfectionSeries->pen();
    pen.setColor(QColor(255, 0 , 0));
    currentInfectionSeries->setPen(pen);

    m_chartCurrentInfections->setTitle(QString::fromStdString("Infections"));
    m_chartCurrentInfections->setPlotAreaBackgroundBrush(QBrush(Qt::white));
    m_chartCurrentInfections->setPlotAreaBackgroundVisible(true);
    m_chartCurrentInfections->setBackgroundRoundness(0);
    m_chartCurrentInfections->setMargins(QMargins(0, 0, 0, 0));

    // Set x and y axis
    xAxisDays->setTitleText("Time [days]");
    xAxisDays->setRange(0, static_cast<int>(parameters.numberOfDays * 1.3));
    yAxisInfections->setTitleText("Number of infected");
    yAxisInfections->setRange(0, static_cast<int>(parameters.numberOfInfections * 1.3));


    m_chartCurrentInfections->addAxis(xAxisDays.data(), Qt::AlignBottom);
    m_chartCurrentInfections->addAxis(yAxisInfections.data(), Qt::AlignLeft);

    currentInfectionSeries->attachAxis(xAxisDays.data());
    currentInfectionSeries->attachAxis(yAxisInfections.data());
    ui->widget_chartCurrentlyInfected->setRenderHint(QPainter::Antialiasing);
    ui->widget_chartCurrentlyInfected->setChart(m_chartCurrentInfections.data());


    //Total Chart
    m_chartTotalParameters->legend()->hide();
    m_chartTotalParameters->addSeries(totalDeadSeries.get());
    m_chartTotalParameters->addSeries(totalInfectionSeries.get());
    m_chartTotalParameters->addSeries(totalRecoveredSeries.get());

    QPen infectedPen = totalInfectionSeries->pen();
    QPen recoveredPen = totalRecoveredSeries->pen();
    QPen deadPen = totalDeadSeries->pen();
    infectedPen.setColor(QColor(255, 0 , 0));
    recoveredPen.setColor(QColor(0, 255 , 50));
    deadPen.setColor(QColor(0, 0 , 0));
    totalInfectionSeries->setPen(infectedPen);
    totalRecoveredSeries->setPen(recoveredPen);
    totalDeadSeries->setPen(deadPen);

    m_chartTotalParameters->setTitle(QString::fromStdString("Total infected, dead and recovered"));
    m_chartTotalParameters->setPlotAreaBackgroundBrush(QBrush(Qt::white));
    m_chartTotalParameters->setPlotAreaBackgroundVisible(true);
    m_chartTotalParameters->setBackgroundRoundness(0);
    m_chartTotalParameters->setMargins(QMargins(0, 0, 0, 0));

    // Set x and y axis
    yAxisTotal->setTitleText("Number of people");
    yAxisTotal->setRange(0, static_cast<int>(parameters.numberOfTotalInfections * 1.3));


    m_chartTotalParameters->addAxis(xAxisDays.data(), Qt::AlignBottom);
    m_chartTotalParameters->addAxis(yAxisTotal.data(), Qt::AlignLeft);

    totalInfectionSeries->attachAxis(xAxisDays.data());
    totalRecoveredSeries->attachAxis(xAxisDays.data());
    totalDeadSeries->attachAxis(xAxisDays.data());
    totalInfectionSeries->attachAxis(yAxisTotal.data());
    totalRecoveredSeries->attachAxis(yAxisTotal.data());
    totalDeadSeries->attachAxis(yAxisTotal.data());
    ui->widget_chartTotalParameters->setRenderHint(QPainter::Antialiasing);
    ui->widget_chartTotalParameters->setChart(m_chartTotalParameters.data());


    // Daily chart
    m_chartDailyInfections->legend()->hide();
    m_chartDailyInfections->addSeries(dailyInfectionSeries.get());
    dailyInfectionSeries->append(dailyInfectedSet.get());

    QPen penDaily = dailyInfectionSeries->pen();
    pen.setColor(QColor(255, 127, 0));
    dailyInfectionSeries->setPen(penDaily);

    m_chartDailyInfections->setTitle(QString::fromStdString("Infections"));
    m_chartDailyInfections->setPlotAreaBackgroundBrush(QBrush(Qt::white));
    m_chartDailyInfections->setPlotAreaBackgroundVisible(true);
    m_chartDailyInfections->setBackgroundRoundness(0);
    m_chartDailyInfections->setMargins(QMargins(0, 0, 0, 0));

    // Set x and y axis
    xAxisDays->setTitleText("Time [days]");
    xAxisDays->setRange(0, static_cast<int>(parameters.numberOfDays * 1.3));
    yAxisInfections->setTitleText("Number of infected");
    yAxisInfections->setRange(0, static_cast<int>(parameters.numberOfInfections * 1.3));


    m_chartDailyInfections->addAxis(xAxisDays.data(), Qt::AlignBottom);
    m_chartDailyInfections->addAxis(yAxisInfections.data(), Qt::AlignLeft);

    dailyInfectionSeries->attachAxis(xAxisDays.data());
    dailyInfectionSeries->attachAxis(yAxisInfections.data());
    ui->widget_chartCurrentlyInfected->setRenderHint(QPainter::Antialiasing);
    ui->widget_chartCurrentlyInfected->setChart(m_chartDailyInfections.data());

}

void MainWindow::updateInputParameters()
{
    qDebug() << "Updating input parameters";

    InputPerameters inputParameters;
    inputParameters.incubationPeriodMean = ui->doubleSpinBox_incubationPeriodMean->value();
    inputParameters.incubationPeriodSigma = ui->doubleSpinBox_incubationPeriodSigma->value();
    inputParameters.mildSymptomsPeriodMean = ui->doubleSpinBox_mildIllnessPeriodMean->value();
    inputParameters.mildSymptomsPeriodSigma = ui->doubleSpinBox_mildIllnessPeriodSigma->value();
    inputParameters.severeSymptomsPeriodMean = ui->doubleSpinBox_severeIllnessPeriodMean->value();
    inputParameters.severeSymptomsPeriodSigma = ui->doubleSpinBox_severeIllnessPeriodSigma->value();
    inputParameters.transmitionRateMin = ui->doubleSpinBox_transmissionRateMin->value();
    inputParameters.transmisionRateMax = ui->doubleSpinBox_transmissionRateMax->value();
    inputParameters.deathRateMin = ui->doubleSpinBox_deathRateMin->value();
    inputParameters.deathRateMax = ui->doubleSpinBox_deathRateMax->value();
    inputParameters.persentSevereCases = ui->doubleSpinBox_percentSevereCases->value();

    simulation->UpdateInputParameters(&inputParameters);

    // persons property - m_probabilityToInfect = (m_incubationPeriod + m_illnessPeriod) / m_infectionRate;

}

void MainWindow::updateOutputParametersOnGUI(const OutputParameters *outputParameters)
{
    ui->label_indicator_numberOfInfections->setNum(outputParameters->numberOfInfections);
    ui->label_indicator_numberOfDeaths->setNum(outputParameters->numberOfDeaths);
    ui->label_indicator_numberOfDays->setNum(outputParameters->numberOfDays);
    ui->label_indicator_numberOfMildCases->setNum(outputParameters->numberOfMildSymptoms);
    ui->label_indicator_numberOfSevereCases->setNum(outputParameters->numberOfSevereSymptoms);
    ui->label_indicator_numberOfTotalInfections->setNum(outputParameters->numberOfTotalInfections);
    ui->label_indicator_numberOfRecovered->setNum(outputParameters->numberOfRecovered);


    static int maxNumOfInfections = 0;
    currentInfectionSeries->append(outputParameters->numberOfDays, outputParameters->numberOfInfections);

    xAxisDays->setMax(outputParameters->numberOfDays + 0.1*outputParameters->numberOfDays);

    if (outputParameters->numberOfInfections > maxNumOfInfections)
        maxNumOfInfections = outputParameters->numberOfInfections;
    yAxisInfections->setMax(maxNumOfInfections + 0.1*maxNumOfInfections);
}

void MainWindow::reset()
{
    simulation->restart();
    populationMap->clean();

    currentInfectionSeries->clear();
}

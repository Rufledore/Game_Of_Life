#include <QGridLayout>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QDebug>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "entity.h"
#include "populationmap.h"
#include "globals.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    cellsPerRow(Constants::numberOfCellsPerRow),
    m_chartCurrentInfections(new QtCharts::QChart),
    currentInfectionsSeries(new QtCharts::QLineSeries),
    dailyNewInfectionsSeries(new QtCharts::QLineSeries),
    yAxisInfections(new QtCharts::QValueAxis),
    xAxisInfections(new QtCharts::QValueAxis),
    m_chartTotalParameters(new QtCharts::QChart),
    totalInfectionSeries(new QtCharts::QLineSeries),
    totalRecoveredSeries(new QtCharts::QLineSeries),
    totalDeadSeries(new QtCharts::QLineSeries),
    yAxisTotal(new QtCharts::QValueAxis),
    xAxisTotal(new QtCharts::QValueAxis),
    m_chartDailyInfections(new QtCharts::QChart),
    dailyInfectedSet(new QtCharts::QBarSet("Daily cases")),
    dailyInfectionSeries(new QtCharts::QBarSeries),
    yAxisDailyInfected(new QtCharts::QValueAxis),
    xAxisDailyInfected(new QtCharts::QBarCategoryAxis),
    populationMap(new PopulationMap(this, cellsPerRow)),
    simulation(new SimulationCore(nullptr, cellsPerRow))
  /*
   QSharedPointer<QtCharts::QChart> m_chartTotalParameters;
    QSharedPointer<QtCharts::QLineSeries> totalInfectionSeries;
    QSharedPointer<QtCharts::QLineSeries> totalRecoveredSeries;
    QSharedPointer<QtCharts::QLineSeries> totalDeadSeries;
    QSharedPointer<QtCharts::QValueAxis> yAxisTotal;

    QSharedPointer<QtCharts::QChart> m_chartDailyInfections;
    QSharedPointer<QtCharts::QBarSet> dailyInfectedSet;
    QSharedPointer<QtCharts::QBarSeries> dailyInfectionSeries;
    QSharedPointer<QtCharts::QValueAxis> yAxisDailyInfected;


  */
{
    /* TODOs:
     * 2. Separate the infection possibility between close neighbors and random cells from the map.
     *
     *
     */


    ui->setupUi(this);

    setUpChart();

    QWidget::setWindowTitle(m_WindowTitle);

    ui->graphicsView_map->setScene(populationMap.data());
    ui->graphicsView_map->setAlignment(Qt::AlignCenter);
    connect(ui->doubleSpinBox_deathRateMax, &QDoubleSpinBox::editingFinished, this, &MainWindow::updateInputParameters);
    connect(ui->doubleSpinBox_incubationPeriodMean, &QDoubleSpinBox::editingFinished, this, &MainWindow::updateInputParameters);
    connect(ui->doubleSpinBox_incubationPeriodSigma, &QDoubleSpinBox::editingFinished, this, &MainWindow::updateInputParameters);
    connect(ui->doubleSpinBox_severeIllnessPeriodMean, &QDoubleSpinBox::editingFinished, this, &MainWindow::updateInputParameters);
    connect(ui->doubleSpinBox_severeIllnessPeriodSigma, &QDoubleSpinBox::editingFinished, this, &MainWindow::updateInputParameters);
    connect(ui->doubleSpinBox_mildIllnessPeriodMean, &QDoubleSpinBox::editingFinished, this, &MainWindow::updateInputParameters);
    connect(ui->doubleSpinBox_mildIllnessPeriodSigma, &QDoubleSpinBox::editingFinished, this, &MainWindow::updateInputParameters);
    connect(ui->doubleSpinBox_percentSevereCases, &QDoubleSpinBox::editingFinished, this, &MainWindow::updateInputParameters);
//    connect(ui->doubleSpinBox_transmissionRateMin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MainWindow::updateInputParameters);
//    connect(ui->doubleSpinBox_transmissionRateMax, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MainWindow::updateInputParameters);
    connect(ui->doubleSpinBox_deathRateMin, &QDoubleSpinBox::editingFinished, this, &MainWindow::updateInputParameters);
    connect(ui->doubleSpinBox_deathRateMax, &QDoubleSpinBox::editingFinished, this, &MainWindow::updateInputParameters);
//    connect(ui->spinBox_numberOfCellsPerAxis, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::updateNumberOfCellsPerRow);
    connect(ui->spinBox_numberOfCellsPerAxis, &QSpinBox::editingFinished, this, &MainWindow::updateNumberOfCellsPerRow);

    connect(ui->doubleSpinBox_transmissionRateMax, &QDoubleSpinBox::editingFinished, this, &MainWindow::updateTransmissiotnRate);
    connect(ui->doubleSpinBox_transmissionRateMin, &QDoubleSpinBox::editingFinished, this, &MainWindow::updateTransmissiotnRate);



    connect(populationMap.data(), &PopulationMap::clicked, simulation.data(), &SimulationCore::changeClickedPersonState);
    connect(simulation.data(), &SimulationCore::populationStatusUpdated, populationMap.data(), &PopulationMap::updatePopulationStatus);
    connect(simulation.data(), &SimulationCore::updatedOutputParameters, this, &MainWindow::updateOutputParametersOnGUI);

    connect(ui->pushButton_calculateNextDay, &QPushButton::clicked, simulation.data(), &SimulationCore::update);
    connect(ui->pushButton_reset, &QPushButton::clicked, this, &MainWindow::reset);

    connect(ui->pushButton_start, &QPushButton::clicked, simulation.data(), &SimulationCore::start);
    connect(ui->pushButton_stop, &QPushButton::clicked, simulation.data(), &SimulationCore::stop);

    simulation->updateMap();

    // Set default values
    ui->label_indicator_numberOfPopulation->setText(QString::number(cellsPerRow * cellsPerRow));
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
    ui->spinBox_numberOfCellsPerAxis->setValue(Constants::numberOfCellsPerRow);

    updateInputParameters();
    updateTransmissiotnRate();
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

    m_chartCurrentInfections->setVisible(true);
    m_chartCurrentInfections->legend()->setAlignment(Qt::AlignTop);
    m_chartCurrentInfections->addSeries(currentInfectionsSeries.get());
    m_chartCurrentInfections->addSeries(dailyNewInfectionsSeries.get());

    currentInfectionsSeries->setName("Currently infected");
    dailyNewInfectionsSeries->setName("New cases for the day");

    QPen pen = currentInfectionsSeries->pen();
    pen.setColor(QColor(255, 0 , 0));
    currentInfectionsSeries->setPen(pen);
    QPen dailyPen = dailyNewInfectionsSeries->pen();
    dailyPen.setColor(QColor(255, 127 , 0));
    dailyNewInfectionsSeries->setPen(dailyPen);

    m_chartCurrentInfections->setTitle(QString::fromStdString("Currently infected"));
    m_chartCurrentInfections->setPlotAreaBackgroundBrush(QBrush(Qt::white));
    m_chartCurrentInfections->setPlotAreaBackgroundVisible(true);
    m_chartCurrentInfections->setBackgroundRoundness(0);
    m_chartCurrentInfections->setMargins(QMargins(0, 0, 0, 0));

    // Set x and y axis
    xAxisInfections->setTitleText("Time [days]");
    xAxisInfections->setRange(0, static_cast<int>(parameters.numberOfDays * 1.3));
    yAxisInfections->setTitleText("Current number of cases");
    yAxisInfections->setRange(0, static_cast<int>(parameters.numberOfInfections * 1.3));
    yAxisDailyInfected->setTitleText("New cases");


    m_chartCurrentInfections->addAxis(xAxisInfections.data(), Qt::AlignBottom);
    m_chartCurrentInfections->addAxis(yAxisInfections.data(), Qt::AlignLeft);
    m_chartCurrentInfections->addAxis(yAxisDailyInfected.data(), Qt::AlignRight);

    currentInfectionsSeries->attachAxis(xAxisInfections.data());
    currentInfectionsSeries->attachAxis(yAxisInfections.data());
    dailyNewInfectionsSeries->attachAxis(xAxisInfections.data());
    dailyNewInfectionsSeries->attachAxis(yAxisDailyInfected.data());
    ui->widget_chartCurrentlyInfected->setRenderHint(QPainter::Antialiasing);
    ui->widget_chartCurrentlyInfected->setChart(m_chartCurrentInfections.data());

    //Total Chart
//    m_chartTotalParameters->legend()->hide();
    m_chartTotalParameters->setVisible(true);
    m_chartTotalParameters->legend()->setAlignment(Qt::AlignTop);
    m_chartTotalParameters->addSeries(totalDeadSeries.get());
    m_chartTotalParameters->addSeries(totalInfectionSeries.get());
    m_chartTotalParameters->addSeries(totalRecoveredSeries.get());

    totalInfectionSeries->setName("Total number of cases");
    totalRecoveredSeries->setName("Recovered");
    totalDeadSeries->setName("Dead");

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
    xAxisTotal->setTitleText("Time [days]");
    xAxisTotal->setRange(0, static_cast<int>(parameters.numberOfDays * 1.3));


    m_chartTotalParameters->addAxis(xAxisTotal.data(), Qt::AlignBottom);
    m_chartTotalParameters->addAxis(yAxisTotal.data(), Qt::AlignLeft);

    totalInfectionSeries->attachAxis(xAxisTotal.data());
    totalRecoveredSeries->attachAxis(xAxisTotal.data());
    totalDeadSeries->attachAxis(xAxisTotal.data());
    totalInfectionSeries->attachAxis(yAxisTotal.data());
    totalRecoveredSeries->attachAxis(yAxisTotal.data());
    totalDeadSeries->attachAxis(yAxisTotal.data());
    ui->widget_chartTotalParameters->setRenderHint(QPainter::Antialiasing);
    ui->widget_chartTotalParameters->setChart(m_chartTotalParameters.data());


    totalDeadSeries->append(0, 0);
    totalInfectionSeries->append(0, 0);
    totalRecoveredSeries->append(0, 0);
    currentInfectionsSeries->append(0, 0);
    dailyNewInfectionsSeries->append(0, 0);

// TODO the bar chart that should present the daily number of new cases cause a segmentation fault.
//    that should be investigated
/*
    // Daily chart
    m_chartDailyInfections->legend()->hide();
    dailyInfectionSeries->append(dailyInfectedSet.get());
    m_chartDailyInfections->addSeries(dailyInfectionSeries.get());
    m_chartDailyInfections->setAnimationOptions(QChart::SeriesAnimations);

    QPen penDaily = dailyInfectedSet->pen();
    penDaily.setColor(QColor(255, 127, 0));
    dailyInfectedSet->setPen(penDaily);

    m_chartDailyInfections->setTitle(QString::fromStdString("Infections per day"));
    m_chartDailyInfections->setPlotAreaBackgroundBrush(QBrush(Qt::white));
    m_chartDailyInfections->setPlotAreaBackgroundVisible(true);
    m_chartDailyInfections->setBackgroundRoundness(0);
    m_chartDailyInfections->setMargins(QMargins(0, 0, 0, 0));

    // Set x and y axis
    xAxisDailyInfected->setTitleText("Time [days]");
    QStringList vals = {"1", "2", "3"};
    xAxisDailyInfected->append(vals);
//    xAxisDailyInfected->setRange(0, static_cast<int>(parameters.numberOfDays * 1.3));
    yAxisDailyInfected->setTitleText("Number of cases");
    yAxisDailyInfected->setRange(0, static_cast<int>(parameters.numberOfInfections * 1.3)); // These ranges are unnecessary


    m_chartDailyInfections->addAxis(xAxisDailyInfected.data(), Qt::AlignBottom);
    dailyInfectionSeries->attachAxis(xAxisDailyInfected.data());

    m_chartDailyInfections->addAxis(yAxisDailyInfected.data(), Qt::AlignLeft);
    dailyInfectionSeries->attachAxis(yAxisDailyInfected.data());

    ui->widget_chartDailyInfections->setRenderHint(QPainter::Antialiasing);
    ui->widget_chartDailyInfections->setChart(m_chartDailyInfections.data());
*/
}

void MainWindow::updateTransmissiotnRate()
{

    double transmissionRateMin = ui->doubleSpinBox_transmissionRateMin->value();
    double transmissionRateMax = ui->doubleSpinBox_transmissionRateMax->value();
    if (transmissionRateMin >= transmissionRateMax) {
        ui->doubleSpinBox_transmissionRateMax->blockSignals(true);
        transmissionRateMax = transmissionRateMin + 0.01;
        ui->doubleSpinBox_transmissionRateMax->setValue(transmissionRateMax);
        ui->doubleSpinBox_transmissionRateMax->blockSignals(false);
    }

    simulation->UpdateTransmissionProbability(transmissionRateMin, transmissionRateMax);
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
    inputParameters.persentSevereCases = ui->doubleSpinBox_percentSevereCases->value();


    double transmissionRateMin = ui->doubleSpinBox_transmissionRateMin->value();
    double transmissionRateMax = ui->doubleSpinBox_transmissionRateMax->value();
    if (transmissionRateMin >= transmissionRateMax) {
        transmissionRateMax = transmissionRateMin + 0.01;
        ui->doubleSpinBox_transmissionRateMax->setValue(transmissionRateMax);
    }
    inputParameters.transmissionRateMin = transmissionRateMin;
    inputParameters.transmissionRateMax = transmissionRateMax;


    double deathRateMin = ui->doubleSpinBox_deathRateMin->value();
    double deathRateMax = ui->doubleSpinBox_deathRateMax->value();
    if (deathRateMin >= deathRateMax) {
        ui->doubleSpinBox_deathRateMax->blockSignals(true);
        deathRateMax = deathRateMin + 0.01;
        ui->doubleSpinBox_deathRateMax->setValue(deathRateMax);
        ui->doubleSpinBox_deathRateMax->blockSignals(false);
    }
    inputParameters.deathRateMax = deathRateMax;
    inputParameters.deathRateMin = deathRateMin;


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
    static int maxNumberOfTotalCases = 0;
    static int maxNumberOfDailyCases = 0;
    static int previousNumberOfTotalCases = 0;

    //Reset the static variables if the simulations has been reset.
    if (outputParameters->numberOfDays <= 1) {
        maxNumOfInfections = 0;
        maxNumberOfDailyCases = 0;
        maxNumberOfTotalCases = 0;
        previousNumberOfTotalCases = 0;
    }

    int numberOfDailyCases = outputParameters->numberOfTotalInfections - previousNumberOfTotalCases;
    previousNumberOfTotalCases = outputParameters->numberOfTotalInfections;

    currentInfectionsSeries->append(outputParameters->numberOfDays, outputParameters->numberOfInfections);
    dailyNewInfectionsSeries->append(outputParameters->numberOfDays, numberOfDailyCases);

    xAxisInfections->setMax(outputParameters->numberOfDays + 0.1*outputParameters->numberOfDays);

    if (outputParameters->numberOfInfections > maxNumOfInfections)
        maxNumOfInfections = outputParameters->numberOfInfections;
    yAxisInfections->setMax(maxNumOfInfections + 0.1*maxNumOfInfections);

    if (numberOfDailyCases > maxNumberOfDailyCases)
        maxNumberOfDailyCases = numberOfDailyCases;
    yAxisDailyInfected->setMax(maxNumberOfDailyCases + 0.1*maxNumberOfDailyCases);

    ////////////////////////////
    totalDeadSeries->append(outputParameters->numberOfDays, outputParameters->numberOfDeaths);
    totalRecoveredSeries->append(outputParameters->numberOfDays, outputParameters->numberOfRecovered);
    totalInfectionSeries->append(outputParameters->numberOfDays, outputParameters->numberOfTotalInfections);


    xAxisTotal->setMax(outputParameters->numberOfDays + 0.1*outputParameters->numberOfDays);

    if (outputParameters->numberOfTotalInfections > maxNumberOfTotalCases)
        maxNumberOfTotalCases = outputParameters->numberOfTotalInfections;
    yAxisTotal->setMax(maxNumberOfTotalCases + 0.1*maxNumberOfTotalCases);

    ////////////////////

}

void MainWindow::updateNumberOfCellsPerRow()
{
    cellsPerRow = ui->spinBox_numberOfCellsPerAxis->value();

    if (cellsPerRow < 10) {
        ui->spinBox_numberOfCellsPerAxis->setValue(10);
        cellsPerRow = ui->spinBox_numberOfCellsPerAxis->value();
    }
    else if (cellsPerRow > 200) {
        ui->spinBox_numberOfCellsPerAxis->setValue(200);
        cellsPerRow = ui->spinBox_numberOfCellsPerAxis->value();
    }

}

void MainWindow::reset()
{
    simulation->restart(cellsPerRow);
    populationMap->restart(cellsPerRow);


    ui->label_indicator_numberOfPopulation->setText(QString::number(cellsPerRow * cellsPerRow));

    currentInfectionsSeries->clear();
    totalInfectionSeries->clear();
    totalRecoveredSeries->clear();
    totalDeadSeries->clear();
    dailyNewInfectionsSeries->clear();
}

void MainWindow::startSimulation()
{
    /*
    simulation->isRunning = true;

        QScopedPointer<SimulationRunner> runner(new SimulationRunner);
        connect(runner.data(), &SimulationRunner::finished, runner.data(), &QObject::deleteLater);
        runner->startSimulation(simulation);
    */

}

void MainWindow::stopSimulation()
{
//    simulation->isRunning = false;
}

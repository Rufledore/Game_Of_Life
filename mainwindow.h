#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QHash>
#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>

#include "game.h"
#include "populationmap.h"
#include "person.h"
#include "simulationcore.h"
#include "globals.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    // Infection Chart
    QSharedPointer<QtCharts::QChart> m_chartCurrentInfections;
    QSharedPointer<QtCharts::QLineSeries> currentInfectionsSeries;
    QSharedPointer<QtCharts::QLineSeries> dailyNewInfectionsSeries;
    QSharedPointer<QtCharts::QValueAxis> yAxisInfections;
    QSharedPointer<QtCharts::QValueAxis> xAxisInfections;

    QSharedPointer<QtCharts::QChart> m_chartTotalParameters;
    QSharedPointer<QtCharts::QLineSeries> totalInfectionSeries;
    QSharedPointer<QtCharts::QLineSeries> totalRecoveredSeries;
    QSharedPointer<QtCharts::QLineSeries> totalDeadSeries;
    QSharedPointer<QtCharts::QValueAxis> yAxisTotal;
    QSharedPointer<QtCharts::QValueAxis> xAxisTotal;

    QSharedPointer<QtCharts::QChart> m_chartDailyInfections;
    QSharedPointer<QtCharts::QBarSet> dailyInfectedSet;
    QSharedPointer<QtCharts::QBarSeries> dailyInfectionSeries;
    QSharedPointer<QtCharts::QValueAxis> yAxisDailyInfected;
    QSharedPointer<QtCharts::QBarCategoryAxis> xAxisDailyInfected;

    // Graphical map
    QSharedPointer<PopulationMap> populationMap;

    // The background object that runs the simulation.
    QSharedPointer<SimulationCore> simulation;

    // Methods ---------------------------------------------------------------------------------------------------------
    void setUpChart();

private slots:
    void updateInputParameters();
    void updateOutputParametersOnGUI(const OutputParameters* outputParameters);
    void reset();

};

#endif // MAINWINDOW_H

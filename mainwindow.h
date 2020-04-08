#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QHash>
#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>

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
    QSharedPointer<QtCharts::QChart> m_chart;
    QSharedPointer<QtCharts::QLineSeries> infectionSeries;
    QSharedPointer<QtCharts::QValueAxis> yAxis;
    QSharedPointer<QtCharts::QValueAxis> xAxis;

    // Graphical map
    QSharedPointer<PopulationMap> populationMap;

    // The background object that runs the simulation.
    QSharedPointer<SimulationCore> runner;

    // Methods ---------------------------------------------------------------------------------------------------------
    void setUpChart();

private slots:
    void updateInputParameters();
    void updateOutputParametersOnGUI(const OutputParameters* outputParameters);

};

#endif // MAINWINDOW_H

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QHash>
#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>

#include "game.h"
#include "gamemap.h"
#include "person.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

typedef QPair<int, int> CellCoordinates;
typedef QHash<CellCoordinates, Person> InfectionMap;


public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    // Simulation parameters

    // Input parameters
    double m_incubationPeriod;
    double m_illnessPeriod;
    double m_infectionRate;
    double m_deathRate;

    // Calculated initial parameters
    double m_probabilityToInfect;

    // Infection Chart
    QSharedPointer<QtCharts::QChart> m_chart;
    QSharedPointer<QtCharts::QLineSeries> infectionSeries;
    QSharedPointer<QtCharts::QValueAxis> yAxis;
    QSharedPointer<QtCharts::QValueAxis> xAxis;



    // Output parameters
    int m_numberOfInfected = 0;
    int m_numberOfDays = 0;

    // Background and functional parameters
    QSharedPointer<InfectionMap> infectedPopulationMap;               //A table with vitaliti states of each cell of the map. The key is QPait of coordinates.
    QSharedPointer<InfectionMap> newGenerationBackgroundMap;  //A table with vitaliti states of each cell of the map. The key is QPait of coordinates.

    // Graphical map
    int m_numberOfCellsPerRow = 129;
    QSharedPointer<PopulationMap> populationMap;

    // Methods ---------------------------------------------------------------------------------------------------------
    void setUpChart();
    void calculateNextDay();
    void updateOutpus();


private slots:
    void updateInitialParameters();
    void changePersonState(CellCoordinates cell);

};

#endif // MAINWINDOW_H

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QHash>

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
    // Simulation parameters

    // Input parameters
    double m_incubationPeriod;
    double m_illnessPeriod;
    double m_infectionRate;
    double m_deathRate;

    // Calculated initial parameters
    double m_probabilityToInfect;



    // Output parameters
    int m_numberOfInfected;
    int m_numberOfDays;

    // Background and functional parameters
    QSharedPointer<InfectionMap> backgroundMap;               //A table with vitaliti states of each cell of the map. The key is QPait of coordinates.
    QSharedPointer<InfectionMap> newGenerationBackgroundMap;  //A table with vitaliti states of each cell of the map. The key is QPait of coordinates.

    Ui::MainWindow *ui;
    Game m_Game;\

    // Methods ---------------------------------------------------------------------------------------------------------
    void calculateNextDay();
    void updateOutpus();


private slots:
    void updateInitialParameters();


};

#endif // MAINWINDOW_H

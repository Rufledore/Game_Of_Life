#include "populationmap.h"
#include <QDebug>
#include <QGraphicsScene>
#include <QPainter>
#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <QGraphicsView>
#include <QHash>
#include <QPoint>
#include <QPair>

PopulationMap::PopulationMap(QWidget *parent, int cellsPerRow) :
    QGraphicsScene(parent),
    m_cellsPerRow(cellsPerRow),
    cellWidth(m_cellsPerRow <= 100 ? m_cellsPerRow <= 20 ? 16 : 6 : 4),
    cellSeparator(cellWidth < 10 ? 1 : cellWidth/10),
    fieldWidth((m_cellsPerRow + 2) * cellWidth + (m_cellsPerRow - 1) * cellSeparator),  //WIdth = (Num of cells * width of cell) + (Num of cells - 1) * separator) + 2 cells for the frame;
    fieldHeight((m_cellsPerRow + 2) * cellWidth + (m_cellsPerRow - 1) * cellSeparator), //(cellsPerRow * (cellWidth + cellSeparator)) + cellWidth * 2 - cellSeparator;
    backgroundMap(new QHash<CellCoordinates, VitalityState>()),                     //Map with vitaliti state of each sqare of the game map
    gamePixmap(new QPixmap(fieldWidth, fieldHeight)),                               //Used as a surface for painting
//    gamePainter(new QPainter(gamePixmap.data())),
    graphicItem(this->addPixmap(*gamePixmap))
{
    /*
    // Set the with of the cell depending to the number of the cells;
    if (m_cellsPerRow < 20) {
        cellWidth = 16;
    } else if (m_cellsPerRow < 100) {
        cellWidth = 8;
    } else if (m_cellsPerRow < 200) {
        cellWidth = 4;
    } else {
        cellWidth = 2;
    }

    // Set the cell separator in depending to the cell width
    cellSeparator = cellWidth < 10 ? 1 : cellWidth/10;

    // Set field parameters
    fieldWidth = (m_cellsPerRow + 2) * cellWidth + (m_cellsPerRow - 1) * cellSeparator;  //WIdth = (Num of cells * width of cell) + (Num of cells - 1) * separator) + 2 cells for the frame;
    fieldHeight = (m_cellsPerRow + 2) * cellWidth + (m_cellsPerRow - 1) * cellSeparator; //(cellsPerRow * (cellWidth + cellSeparator)) + cellWidth * 2 - cellSeparator;

    // Set Qt graphical objects that are need for drawing the map
    gamePixmap.reset(new QPixmap(fieldWidth, fieldHeight));
    gamePainter.reset(new QPainter(gamePixmap.get()));
    graphicItem.reset(this->addPixmap(*gamePixmap));
*/
    this->drawInitialGeneration();

    graphicItem->setPixmap(*gamePixmap);

    connect(this, &PopulationMap::clicked, this, &PopulationMap::changeClickedCell);

}

PopulationMap::~PopulationMap()
{
}

void PopulationMap::OBSOLATE_setInitialMap()
{
    /* Use this function if you want to automatically set initial conditions on the population.

    for (int x = 0; x < cellsPerRow; x++) {
        for (int y = 0; y < cellsPerRow; y++) {
            if (y%2 == 0) {
                backgroundMap->insert(CellCoordinates(x, y), dead);     //Update the background map
            } else {
                backgroundMap->insert(CellCoordinates(x, y), healty);   //Update the background map
            }
        }
    }

    */
    this->drawInitialGeneration();
}


void PopulationMap::drawInitialGeneration()
{
    QScopedPointer<QPainter> painter(new QPainter(gamePixmap.data()));

    QRectF background(QPointF(0, 0), QPointF(fieldWidth, fieldHeight));
    painter->fillRect(background, backgroundBrush);

    for (int row = 0; row < m_cellsPerRow; ++row) {
        for (int col = 0; col < m_cellsPerRow; ++col) {
            switch (backgroundMap->value(CellCoordinates(col, row))) {
            case VitalityState::healthy:
                drawNthCell(CellCoordinates(col, row), healthyColorBrush, painter.get());
                break;
            case VitalityState::infected_incubation:
                drawNthCell(CellCoordinates(col, row), incubationColorBrush, painter.get());
                break;
            case VitalityState::infected_mild_symptoms:
                drawNthCell(CellCoordinates(col, row), mildColorBrush, painter.get());
                break;
            case VitalityState::infected_severe_symptoms:
                drawNthCell(CellCoordinates(col, row), severeColorBrush, painter.get());
                break;
            case VitalityState::dead:
                drawNthCell(CellCoordinates(col, row), deadColorBrush, painter.get());
                break;
            default:
                drawNthCell(CellCoordinates(col, row), healthyColorBrush, painter.get());
            }
        }
    }
}

void PopulationMap::drawNextGeneration(const InfectionMap *infectedPopulation)
{
    QScopedPointer<QPainter> painter(new QPainter(gamePixmap.data()));
    InfectionMap::const_iterator infectedPerson;
    for (infectedPerson = infectedPopulation->constBegin();
         infectedPerson != infectedPopulation->constEnd();
         ++infectedPerson) {

        CellCoordinates personPosition = infectedPerson.key();
        Person person = infectedPerson.value();

        if (person.vitalityState != backgroundMap->value(personPosition)) {      //If this person is the with same state in
            switch (person.vitalityState) {
            case VitalityState::healthy:
                drawNthCell(personPosition, healthyColorBrush, painter.get());
                backgroundMap->insert(personPosition, VitalityState::healthy);
                break;
            case VitalityState::infected_incubation:
                drawNthCell(personPosition, incubationColorBrush, painter.get());
                backgroundMap->insert(personPosition, VitalityState::infected_incubation);
                break;
            case VitalityState::infected_mild_symptoms:
                drawNthCell(personPosition, mildColorBrush, painter.get());
                backgroundMap->insert(personPosition, VitalityState::infected_mild_symptoms);
                break;
            case VitalityState::infected_severe_symptoms:
                drawNthCell(personPosition, severeColorBrush, painter.get());
                backgroundMap->insert(personPosition, VitalityState::infected_severe_symptoms);
                break;
            case VitalityState::dead:
                drawNthCell(personPosition, deadColorBrush, painter.get());
                backgroundMap->insert(personPosition, VitalityState::dead);
                break;
            default:
                drawNthCell(personPosition, healthyColorBrush, painter.get());
                backgroundMap->insert(personPosition, VitalityState::healthy);
            }
        }
    }
    graphicItem->setPixmap(*gamePixmap);
}

void PopulationMap::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    clickedPoint = event->scenePos().toPoint();
    qDebug() << clickedPoint;
    int x = (clickedPoint.x() - cellWidth) / (cellWidth + cellSeparator);
    if (x >= m_cellsPerRow) x = m_cellsPerRow - 1;
    int y = (clickedPoint.y() - cellWidth) / (cellWidth + cellSeparator);
    if (y >= m_cellsPerRow) y = m_cellsPerRow - 1;

    clicked(CellCoordinates(x, y));
}

void PopulationMap::changeClickedCell(CellCoordinates cell)
{
//    QPoint mouseClick = mouseEvent->scenePos().toPoint();

//    int x = (mouseClick.x() - cellWidth) / (cellWidth + cellSeparator);
//    int y = (mouseClick.y() - cellWidth) / (cellWidth + cellSeparator);


    QScopedPointer<QPainter> painter(new QPainter(gamePixmap.data()));
    VitalityState personState = backgroundMap->value(cell);

    switch (personState) {
    case VitalityState::healthy:
        (*backgroundMap)[cell] = VitalityState::infected_incubation;
        drawNthCell(cell, incubationColorBrush, painter.get());
        break;
    case VitalityState::infected_incubation:
        (*backgroundMap)[cell] = VitalityState::infected_mild_symptoms;
        drawNthCell(cell, mildColorBrush, painter.get());
        break;
    case VitalityState::infected_mild_symptoms:
        (*backgroundMap)[cell] = VitalityState::infected_severe_symptoms;
        drawNthCell(cell, severeColorBrush, painter.get());
        break;
    case VitalityState::infected_severe_symptoms:
        (*backgroundMap)[cell] = VitalityState::dead;
        drawNthCell(cell, deadColorBrush, painter.get());
        break;
    case VitalityState::dead:
        (*backgroundMap)[cell] = VitalityState::healthy;
        drawNthCell(cell, healthyColorBrush, painter.get());
        break;
    default:
        (*backgroundMap)[cell] = VitalityState::healthy;
        drawNthCell(cell, healthyColorBrush, painter.get());
    }

/*
    if (backgroundMap->value(CellCoordinates(x, y)) == dead) {     //When is white
        drawNthCell(QPoint(x, y), blackBrush);
        backgroundMap->insert(CellCoordinates(x, y), alive);
    } else {                                                       //when is black
        drawNthCell(QPoint(x, y), whiteBrush);
        backgroundMap->insert(CellCoordinates(x, y), dead);
    }
*/
//    this->addPixmap(*gamePixmap);
    graphicItem->setPixmap(*gamePixmap);
}

void PopulationMap::updatePopulationStatus(const InfectionMap *map)
{
    drawNextGeneration(map);
}

void PopulationMap::drawNthCell(CellCoordinates currentCell, QBrush brush, QPainter* painter)
{
    int x = currentCell.first;
    int y = currentCell.second;

    QPoint topLeft(0, 0);
    QPoint bottomRight(0, 0);


    topLeft.setX((x + 1) * cellWidth + x * cellSeparator);
    topLeft.setY((y + 1) * cellWidth + y * cellSeparator);

    bottomRight.setX(topLeft.x() + cellWidth);
    bottomRight.setY(topLeft.y() + cellWidth);

    QRectF rectToDraw(topLeft, bottomRight);
    painter->fillRect(rectToDraw, brush);

//    gamePainter->fillRect(rectToDraw, brush);

}

void PopulationMap::restart(int cellsPerRow)
{
    m_cellsPerRow = cellsPerRow;
    cellWidth = m_cellsPerRow <= 2000 ? m_cellsPerRow <= 100 ? m_cellsPerRow <= 20 ? 16 : 6 : 4 : 2;
    cellSeparator = cellWidth < 10 ? 1 : cellWidth/10;
    fieldWidth = (m_cellsPerRow + 2) * cellWidth + (m_cellsPerRow - 1) * cellSeparator;  //WIdth = (Num of cells * width of cell) + (Num of cells - 1) * separator) + 2 cells for the frame;
    fieldHeight = (m_cellsPerRow + 2) * cellWidth + (m_cellsPerRow - 1) * cellSeparator; //(cellsPerRow * (cellWidth + cellSeparator)) + cellWidth * 2 - cellSeparator;

    QPixmap* newPixmap = new QPixmap(gamePixmap->scaled(fieldWidth, fieldHeight));
    gamePixmap.clear();
    gamePixmap.reset(newPixmap);

//    gamePainter.clear();
//    gamePainter.reset(new QPainter(gamePixmap.data()));
    graphicItem.reset(this->addPixmap(*gamePixmap));


    backgroundMap->clear();
    this->drawInitialGeneration();
    graphicItem->setPixmap(*gamePixmap);
}

void PopulationMap::OBSOLATE_updateNextGeneration()
{
// foreach can be implemented
//    QHash<CellCoordinates, vitalityState> temporaryMap = *backgroundMap;
/*
    for (int row = 0; row < cellsPerRow; ++row) {
        for (int col = 0; col < cellsPerRow; ++col) {
            updateCellState(CellCoordinates(col, row));
        }
    }

    this->drawGeneration(next);

    backgroundMap.swap(newGenerationBackgroundMap);
    newGenerationBackgroundMap =
            QSharedPointer<QHash<CellCoordinates, vitalityState>>(new QHash<CellCoordinates, vitalityState>());
*/
}

void PopulationMap::OBSOLATE_updateCellState(CellCoordinates currentCell)
{
    /*
    int lifeCounter = 0;                                                //The algorithm will always count the current cell

    for (int colCorrection = -1; colCorrection <= 1; ++colCorrection)
    {
        for (int rowCorrection = -1; rowCorrection <= 1; ++rowCorrection)
        {
            if (backgroundMap->value(
                        CellCoordinates((currentCell.first + colCorrection), (currentCell.second + rowCorrection))) == alive &&
                        !(colCorrection == 0 && rowCorrection == 0)) {   //Eliminates the chanse to count the current cell too
                ++lifeCounter;
            }
        }
    }

    if (backgroundMap->value(currentCell) == alive) {                    //If the cell is alive
        if (lifeCounter < 2 || lifeCounter > 3) {                        //If has too few or too many nighbors
            newGenerationBackgroundMap->insert(currentCell, dead);       //The cell dies
        }
        else {
            newGenerationBackgroundMap->insert(currentCell, alive);
        }
    }
    else {                                                               //If the cell is dead
        if (lifeCounter == 3) {                                          //If has 3 live nighbors
            newGenerationBackgroundMap->insert(currentCell, alive);      //The cell becomes alive
        }
    }
    */
}

QPoint PopulationMap::getPixelInCurrentCell(QPoint currentCell)
{
    QPoint pixelInCell(0,0);
    pixelInCell.setX((currentCell.x() + 1) * cellWidth + (currentCell.x() * cellSeparator + (cellWidth / 2)));
    pixelInCell.setY((currentCell.y() + 1) * cellWidth + (currentCell.y() * cellSeparator + (cellWidth / 2)));

    return pixelInCell;
}


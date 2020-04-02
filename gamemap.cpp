#include "gamemap.h"
#include <QDebug>
#include <QGraphicsScene>
#include <QPainter>
#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <QGraphicsView>
#include <QHash>
#include <QPoint>
#include <QPair>

PopulationMap::PopulationMap(QWidget *parent) :
    QGraphicsScene(parent),
    cellsPerRow(129),
    cellWidth(6),
    cellSeparator(cellWidth < 10 ? 1 : cellWidth/10),
    fieldWidth((cellsPerRow + 2) * cellWidth + (cellsPerRow - 1) * cellSeparator),  //WIdth = (Num of cells * width of cell) + (Num of cells - 1) * separator) + 2 cells for the frame;
    fieldHeight((cellsPerRow + 2) * cellWidth + (cellsPerRow - 1) * cellSeparator), //(cellsPerRow * (cellWidth + cellSeparator)) + cellWidth * 2 - cellSeparator;
    backgroundMap(new QHash<CellCoordinates, VitalityState>()),                     //Map with vitaliti state of each sqare of the game map
//    newGenerationBackgroundMap(new QHash<CellCoordinates, vitalityState>()),            //Map for modifying when the next generation is calculated
//    gameGraphicView(new QGraphicsView(this, parent)),                               //Used for visualising of the scene
    gamePixmap(new QPixmap(fieldWidth, fieldHeight)),                               //Used as a surface for painting
    gamePainter(new QPainter(gamePixmap.data())),
    graphicItem(this->addPixmap(*gamePixmap))
//    whiteBrush(new QBrush(QColor(255,255,255))),
//    blackBrush(new QBrush(QColor(0,0,0))),
//    mouseEvent(nullptr)
{
//    gameGraphicView->show();
//    gameGraphicView->resize(fieldWidth + cellWidth, fieldHeight + cellWidth);
//    gameGraphicView->resize(950, 950);

//    this->addPixmap(*gamePixmap);

//    this->setInitialMap();
    this->drawInitialGeneration();

    graphicItem->setPixmap(*gamePixmap);

    connect(this, &PopulationMap::clicked, this, &PopulationMap::changeClickedCell);

}

PopulationMap::~PopulationMap()
{
//    delete mouseEvent;
}

void PopulationMap::setInitialMap()
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
    for (int row = 0; row < cellsPerRow; ++row) {
        for (int col = 0; col < cellsPerRow; ++col) {
            switch (backgroundMap->value(CellCoordinates(col, row))) {
            case VitalityState::healty:
                drawNthCell(QPoint(col, row), whiteBrush);
                break;
            case VitalityState::infected_incubation:
                drawNthCell(QPoint(col, row), yellowBrush);
                break;
            case VitalityState::infected_sick:
                drawNthCell(QPoint(col, row), redBrush);
                break;
            case VitalityState::dead:
                drawNthCell(QPoint(col, row), blackBrush);
                break;
            default:
                drawNthCell(QPoint(col, row), whiteBrush);
            }
        }
    }
}

void PopulationMap::drawNextGeneration(PopulationMap::InfectionMap *populationForDisplay)
{
    for (int row = 0; row < cellsPerRow; ++row) {
        for (int col = 0; col < cellsPerRow; ++col) {
            Person person = populationForDisplay->value(CellCoordinates(col, row));

            if (person.vitalityState != backgroundMap->value(CellCoordinates(col, row))) {      //If this person is the with same state in
                switch (person.vitalityState) {
                case VitalityState::healty:
                    drawNthCell(QPoint(col, row), whiteBrush);
                    break;
                case VitalityState::infected_incubation:
                    drawNthCell(QPoint(col, row), yellowBrush);
                    break;
                case VitalityState::infected_sick:
                    drawNthCell(QPoint(col, row), redBrush);
                    break;
                case VitalityState::dead:
                    drawNthCell(QPoint(col, row), blackBrush);
                    break;
                default:
                    drawNthCell(QPoint(col, row), whiteBrush);
                }                                                                              //next generation it won't be updated.
            }
        }
    }
}

/*
void PopulationMap::drawGeneration(generationType generation)
{
    for (int row = 0; row < cellsPerRow; ++row) {
        for (int col = 0; col < cellsPerRow; ++col) {
            if (generation == initial) {
                if (backgroundMap->value(CellCoordinates(col, row)) == sick) {
                    drawNthCell(QPoint(col, row), blackBrush);
                }
                else {
                    drawNthCell(QPoint(col, row), whiteBrush);
                }
            } else if (generation == next) {
                if (newGenerationBackgroundMap->value(CellCoordinates(col, row)) !=                 //If this cell is the same in the next generation it
                    backgroundMap->value(CellCoordinates(col, row))) {                              //won't be updated.
                    if (newGenerationBackgroundMap->value(CellCoordinates(col, row)) == alive) {
                        drawNthCell(QPoint(col, row), blackBrush);
                    }
                    else {
                        drawNthCell(QPoint(col, row), whiteBrush);
                    }
                }
            }
        }
    }
    graphicItem->setPixmap(*gamePixmap);
}
*/

void PopulationMap::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    clickedPoint = event->scenePos().toPoint();
//    mouseEvent = event;
//    qDebug() << mouseEvent->scenePos();
    qDebug() << clickedPoint;

    clicked();
}

void PopulationMap::changeClickedCell()
{
//    QPoint mouseClick = mouseEvent->scenePos().toPoint();

//    int x = (mouseClick.x() - cellWidth) / (cellWidth + cellSeparator);
//    int y = (mouseClick.y() - cellWidth) / (cellWidth + cellSeparator);
    int x = (clickedPoint.x() - cellWidth) / (cellWidth + cellSeparator);
    int y = (clickedPoint.y() - cellWidth) / (cellWidth + cellSeparator);

    VitalityState personState = backgroundMap->value(CellCoordinates(x, y));

    switch (personState) {
    case VitalityState::healty:
        (*backgroundMap)[CellCoordinates(x, y)] = VitalityState::infected_incubation;
        drawNthCell(QPoint(x, y), yellowBrush);
        break;
    case VitalityState::infected_incubation:
        (*backgroundMap)[CellCoordinates(x, y)] = VitalityState::infected_sick;
        drawNthCell(QPoint(x, y), redBrush);
        break;
    case VitalityState::infected_sick:
        (*backgroundMap)[CellCoordinates(x, y)] = VitalityState::dead;
        drawNthCell(QPoint(x, y), blackBrush);
        break;
    case VitalityState::dead:
        (*backgroundMap)[CellCoordinates(x, y)] = VitalityState::healty;
        drawNthCell(QPoint(x, y), whiteBrush);
        break;
    default:
        drawNthCell(QPoint(x, y), whiteBrush);
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

void PopulationMap::drawNthCell(QPoint currentCell, QBrush brush)
{
    QPoint topLeft(0, 0);
    QPoint bottomRight(0, 0);

    topLeft.setX((currentCell.x() + 1) * cellWidth + (currentCell.x()) * cellSeparator);
    topLeft.setY((currentCell.y() + 1) * cellWidth + (currentCell.y()) * cellSeparator);

    bottomRight.setX(topLeft.x() + cellWidth);
    bottomRight.setY(topLeft.y() + cellWidth);

    QRectF rectToDraw(topLeft, bottomRight);

    gamePainter->fillRect(rectToDraw, brush);

}

void PopulationMap::updateNextGeneration()
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

void PopulationMap::updateCellState(CellCoordinates currentCell)
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


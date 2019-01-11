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

GameMap::GameMap(QWidget *parent) :
    QGraphicsScene(parent),
    cellsPerRow(201),
    cellWidth(5),
    cellSeparator(cellWidth < 10 ? 1 : cellWidth/10),
    fieldWidth((cellsPerRow + 2) * cellWidth + (cellsPerRow - 1) * cellSeparator),  //WIdth = (Num of cells * width of cell) + (Num of cells - 1) * separator) + 2 cells for the frame;
    fieldHeight((cellsPerRow + 2) * cellWidth + (cellsPerRow - 1) * cellSeparator), //(cellsPerRow * (cellWidth + cellSeparator)) + cellWidth * 2 - cellSeparator;
    backgroundMap(new QHash<CellCoordinates, vitalityState>()),                     //Map with vitaliti state of each sqare of the game map
    newGenerationBackgroundMap(new QHash<CellCoordinates, vitalityState>()),            //Map for modifying when the next generation is calculated
    gameGraphicView(new QGraphicsView(this, parent)),                               //Used for visualising of the scene
    gamePixmap(new QPixmap(fieldWidth, fieldHeight)),                               //Used as a surface for painting
    gamePainter(new QPainter(gamePixmap.data())),
    graphicItem(this->addPixmap(*gamePixmap)),
    whiteBrush(new QBrush(QColor(255,255,255))),
    blackBrush(new QBrush(QColor(0,0,0))),
    mouseEvent(nullptr)
{
    gameGraphicView->show();
//    gameGraphicView->resize(fieldWidth + cellWidth, fieldHeight + cellWidth);
    gameGraphicView->resize(1200, 1000);

//    this->addPixmap(*gamePixmap);

    this->setInitialMap();

    graphicItem->setPixmap(*gamePixmap);

    connect(this, &GameMap::clicked, this, &GameMap::changeClickedCell);

}

GameMap::~GameMap()
{
    delete mouseEvent;
}

void GameMap::setInitialMap()
{
    for (int x = 0; x < cellsPerRow; x++) {
        for (int y = 0; y < cellsPerRow; y++) {
            if (y%2 == 0) {
                backgroundMap->insert(CellCoordinates(x, y), dead);     //Update the background map
            } else {
                backgroundMap->insert(CellCoordinates(x, y), alive);     //Update the background map
            }
        }
    }

    this->drawGeneration(initial);
}

void GameMap::drawGeneration(generationType generation)
{
    for (int row = 0; row < cellsPerRow; ++row) {
        for (int col = 0; col < cellsPerRow; ++col) {
            if (generation == initial) {
                if (backgroundMap->value(CellCoordinates(col, row)) == alive) {
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

void GameMap::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    mouseEvent = event;
    qDebug() << mouseEvent->scenePos();

    clicked();
}

void GameMap::changeClickedCell()
{
    QPoint mouseClick = mouseEvent->scenePos().toPoint();

    int x = (mouseClick.x() - cellWidth) / (cellWidth + cellSeparator);
    int y = (mouseClick.y() - cellWidth) / (cellWidth + cellSeparator);

    if(backgroundMap->value(CellCoordinates(x, y)) == dead) {     //When is white
        drawNthCell(QPoint(x, y), blackBrush);
        backgroundMap->insert(CellCoordinates(x, y), alive);
    } else {                                                      //when is black
        drawNthCell(QPoint(x, y), whiteBrush);
        backgroundMap->insert(CellCoordinates(x, y), dead);
    }

//    this->addPixmap(*gamePixmap);
    graphicItem->setPixmap(*gamePixmap);
}

void GameMap::drawNthCell(QPoint currentCell, QSharedPointer<QBrush> brush)
{
    QPoint topLeft(0, 0);
    QPoint bottomRight(0, 0);

    topLeft.setX((currentCell.x() + 1) * cellWidth + (currentCell.x()) * cellSeparator);
    topLeft.setY((currentCell.y() + 1) * cellWidth + (currentCell.y()) * cellSeparator);

    bottomRight.setX(topLeft.x() + cellWidth);
    bottomRight.setY(topLeft.y() + cellWidth);

    QRectF rectToDraw(topLeft, bottomRight);

    gamePainter->fillRect(rectToDraw, *brush);

}

void GameMap::updateNextGeneration()
{
// foreach can be implemented
//    QHash<CellCoordinates, vitalityState> temporaryMap = *backgroundMap;

    for (int row = 0; row < cellsPerRow; ++row) {
        for (int col = 0; col < cellsPerRow; ++col) {
            updateCellState(CellCoordinates(col, row));
        }
    }

    this->drawGeneration(next);

    backgroundMap.swap(newGenerationBackgroundMap);
    newGenerationBackgroundMap =
            QSharedPointer<QHash<CellCoordinates, vitalityState>>(new QHash<CellCoordinates, vitalityState>());


//    this->addPixmap(*gamePixmap);                                       //Update the scene
}

void GameMap::updateCellState(CellCoordinates currentCell)
{
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

}

QPoint GameMap::getPixelInCurrentCell(QPoint currentCell)
{
    QPoint pixelInCell(0,0);
    pixelInCell.setX((currentCell.x() + 1) * cellWidth + (currentCell.x() * cellSeparator + (cellWidth / 2)));
    pixelInCell.setY((currentCell.y() + 1) * cellWidth + (currentCell.y() * cellSeparator + (cellWidth / 2)));

    return pixelInCell;
}


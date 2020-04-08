#ifndef POPULATIONMAP_H
#define POPULATIONMAP_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPoint>
#include <QImage>
#include <QHash>
#include <QPair>
#include <QScopedPointer>
#include <QSharedPointer>

#include "person.h"
#include "globals.h"


class PopulationMap : public QGraphicsScene
{
    Q_OBJECT

public:

    explicit PopulationMap(QWidget *parent = nullptr);
    ~PopulationMap();

    void OBSOLATE_setInitialMap();
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void drawNthCell(CellCoordinates currentCell, QBrush brush);
    void OBSOLATE_updateNextGeneration();
    void OBSOLATE_updateCellState(CellCoordinates currentCell);
    void drawInitialGeneration();
    void drawNextGeneration(const InfectionMap *mapForDisplay);
    QVector<QPoint> getCellsNighbors(CellCoordinates currentCell);
    QPoint getPixelInCurrentCell(QPoint currentCell);

signals:
    void clicked(CellCoordinates cell);

public slots:
    void changeClickedCell(CellCoordinates cell);
    void updatePopulationStatus(const InfectionMap* map);

private:
    int cellsPerRow = Constants::numberOfCellsPerRow;
    int cellWidth;
    int cellSeparator;
    int fieldWidth;
    int fieldHeight;

    QSharedPointer<QHash<CellCoordinates, VitalityState>> backgroundMap;               //A table with vitaliti states of each cell of the map. The key is QPait of coordinates.
    QScopedPointer<QGraphicsScene> gameGraphicScene;                                   //A graphics scebe which present the game map
    QSharedPointer<QPixmap> gamePixmap;                                                //A surface where the game is painted
    QScopedPointer<QPainter> gamePainter;                                              //A tool for paint and repaint the game map
    QScopedPointer<QGraphicsPixmapItem> graphicItem;
    QPoint clickedPoint;
    QBrush healthyColorBrush = QBrush(QColor(200, 200, 200));
    QBrush deadColorBrush = QBrush(QColor(0, 0, 0));
    QBrush severeColorBrush = QBrush(QColor(255, 0, 0));
    QBrush mildColorBrush = QBrush(QColor(250, 174, 21));
    QBrush incubationColorBrush = QBrush(QColor(243, 250, 21));
};


#endif // POPULATIONMAP_H

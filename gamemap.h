#ifndef WIDGET_H
#define WIDGET_H

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


class PopulationMap : public QGraphicsScene
{
    Q_OBJECT

typedef QPair<int, int> CellCoordinates;
typedef QHash<CellCoordinates, Person> InfectionMap;

public:

    explicit PopulationMap(QWidget *parent = nullptr);
    ~PopulationMap();

    void setInitialMap();
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void drawNthCell(QPoint currentCell, QSharedPointer<QBrush> brush);
    void updateNextGeneration();
    void updateCellState(CellCoordinates currentCell);
    void drawInitialGeneration();
    void drawNextGeneration(InfectionMap *mapForDisplay);
//    QVector<QPoint> getPixelsFromCellsNighbors(QPoint currentCell);
    QVector<QPoint> getCellsNighbors(CellCoordinates currentCell);
    QPoint getPixelInCurrentCell(QPoint currentCell);

signals:
    void clicked();

public slots:
    void changeClickedCell();

private:
    enum vitalityState{healty, sick, dead};
    enum generationType{initial, next};

    int cellsPerRow;
    int cellWidth;
    int cellSeparator;
    int fieldWidth;
    int fieldHeight;

//    QSharedPointer<QHash<CellCoordinates, Person>> backgroundMap;               //A table with vitaliti states of each cell of the map. The key is QPait of coordinates.
//    QSharedPointer<QHash<CellCoordinates, Person>> newGenerationBackgroundMap;  //A table with vitaliti states of each cell of the map. The key is QPait of coordinates.
    QSharedPointer<QHash<CellCoordinates, vitalityState>> backgroundMap;               //A table with vitaliti states of each cell of the map. The key is QPait of coordinates.
//    QSharedPointer<QHash<CellCoordinates, vitalityState>> newGenerationBackgroundMap;  //A table with vitaliti states of each cell of the map. The key is QPait of coordinates.
    QScopedPointer<QGraphicsScene> gameGraphicScene;                                   //A graphics scebe which present the game map
    QScopedPointer<QGraphicsView> gameGraphicView;                                     //A graphics view for presenting the game map
    QSharedPointer<QPixmap> gamePixmap;                                                //A surface where the game is painted
    QScopedPointer<QPainter> gamePainter;                                              //A tool for paint and repaint the game map
    QScopedPointer<QGraphicsPixmapItem> graphicItem;
    QSharedPointer<QBrush> whiteBrush;
    QSharedPointer<QBrush> blackBrush;
    QGraphicsSceneMouseEvent* mouseEvent;

    void drawGeneration(generationType generation);
};


#endif // WIDGET_H

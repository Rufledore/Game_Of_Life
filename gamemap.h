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
#include "globals.h"


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
    void drawNthCell(QPoint currentCell, QBrush brush);
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
    enum generationType{initial, next};

    int cellsPerRow;
    int cellWidth;
    int cellSeparator;
    int fieldWidth;
    int fieldHeight;

//    QSharedPointer<QHash<CellCoordinates, Person>> backgroundMap;               //A table with vitaliti states of each cell of the map. The key is QPait of coordinates.
//    QSharedPointer<QHash<CellCoordinates, Person>> newGenerationBackgroundMap;  //A table with vitaliti states of each cell of the map. The key is QPait of coordinates.
    QSharedPointer<QHash<CellCoordinates, VitalityState>> backgroundMap;               //A table with vitaliti states of each cell of the map. The key is QPait of coordinates.
//    QSharedPointer<QHash<CellCoordinates, vitalityState>> newGenerationBackgroundMap;  //A table with vitaliti states of each cell of the map. The key is QPait of coordinates.
    QScopedPointer<QGraphicsScene> gameGraphicScene;                                   //A graphics scebe which present the game map
//    QScopedPointer<QGraphicsView> gameGraphicView;                                     //A graphics view for presenting the game map
    QSharedPointer<QPixmap> gamePixmap;                                                //A surface where the game is painted
    QScopedPointer<QPainter> gamePainter;                                              //A tool for paint and repaint the game map
    QScopedPointer<QGraphicsPixmapItem> graphicItem;
    QBrush whiteBrush = QBrush(QColor(255, 255, 255));
    QBrush blackBrush = QBrush(QColor(0, 0, 0));
    QBrush redBrush = QBrush(QColor(255, 0, 0));
    QBrush yellowBrush = QBrush(QColor(255, 128, 0));
//    QScopedPointer<QGraphicsSceneMouseEvent> mouseEvent;
    QPoint clickedPoint;

    void drawGeneration(generationType generation);
};


#endif // WIDGET_H

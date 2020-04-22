#ifndef BOARD_H
#define BOARD_H

#include "square.h"

#include <QObject>
#include <QGraphicsView>

class Board : public QObject
{
    Q_OBJECT

    private:
        float width_;
        float height_;
        float square_width_;
        float square_height_;
        QGraphicsScene *board_scene_;

    public:
        static const int numRows = 8;
        static const int numCols = 8;

        static Square* board_[numRows][numCols];

        explicit Board(QGraphicsView *view, QObject *parent = nullptr);
        Square* GetSquareAt(Coord c) { return board_[c.X()][c.Y()]; }
        void AddPiece(Piece *piece);
        void MovePiece(Coord from, Coord to);
        void Reset();
};

#endif // BOARD_H

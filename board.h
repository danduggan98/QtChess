#ifndef BOARD_H
#define BOARD_H

#include "square.h"

#include <QObject>
#include <QGraphicsView>
#include <vector>

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

        static Coord wKingPos;
        static Coord bKingPos;

        static Square* board_[numRows][numCols];
        static std::vector<Piece*> pieces;
        static std::vector<Piece*> wCapturedPieces;
        static std::vector<Piece*> bCapturedPieces;

        explicit Board(QGraphicsView *view, QObject *parent = nullptr);
        Square* GetSquareAt(Coord c) { return board_[c.X()][c.Y()]; };
        void AddPiece(Piece *piece);
        void MovePiece(Coord from, Coord to);
        void Reset();

        void DefineMoveset(Piece* p);
        void AddMove(Coord startPos, Coord endPos, std::vector<Coord> &temp_moveset);
        void AddMovespace(Coord startPos, Coord endPos, std::vector<Coord> &temp_moveset);
        void UpdateMovesets();
        void CapturePiece(Piece* p);

        bool ContainsPiece(Coord c) { return !GetSquareAt(c)->isEmpty(); };
        bool ContainsAlly(Coord c1, Coord c2);
        bool ContainsEnemy(Coord c1, Coord c2);
        bool ContainsKing(Coord c);
        bool KingInCheck(char color);
};

#endif // BOARD_H

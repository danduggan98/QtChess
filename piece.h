#ifndef PIECE_H
#define PIECE_H

#include "move.h"

#include <QObject>
#include <QColor>
#include <string>
#include <vector>

class Piece : public QObject {
    Q_OBJECT

    private:
        int x_pos_;
        int y_pos_;
        std::string type_;
        QColor color_;
        std::vector<Move> moveset_();

    public:
        explicit Piece(int x, int y, std::string type, QColor color, QObject *parent = nullptr);
        void DefineMoveset();
        void MovePiece(Move m);
        bool isValidMove(Move m);

    signals:

};

#endif // PIECE_H

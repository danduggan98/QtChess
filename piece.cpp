#include "piece.h"
#include <QDebug>

Piece::Piece(int x, int y, std::string type, QColor color, QObject *parent) : QObject(parent) {
    x_pos_ = x;
    y_pos_ = y;
    type_ = type;
    color_ = color;

    DefineMoveset();
}

//Determine which moves this piece can make given its type
void Piece::DefineMoveset() {

    //Differs based on piece type
    /*if (type_ == "pawn") {
        etc...
    }*/

    qDebug() << "Defined moveset";
}

//Move the piece to a new square
void Piece::MovePiece(Move m) {
    x_pos_ = m.X();
    y_pos_ = m.Y();
}

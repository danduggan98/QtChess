#include "piece.h"
#include <QDebug>

Piece::Piece(int x, int y, QString type, char color, QObject *parent) : QObject(parent) {
    x_pos_ = x;
    y_pos_ = y;
    type_ = type;
    color_ = color;
    moveset_ = {};
}

//Move the piece to a new square
void Piece::ChangePos(Coord m) {
    x_pos_ = m.X();
    y_pos_ = m.Y();
}

//See if a given coordinate is in our moveset, indicating it can be played
bool Piece::IsValidMove(Coord m) {

    //Check that the move is actually on the board
    if (m.X() >= 0 && m.X() < 8 && m.Y() >= 0 && m.Y() < 8) {

        //Look for the move in our moveset
        for (unsigned int i = 0; i < moveset_.size(); i++) {
            if (moveset_[i] == m) {
                return true;
            }
        }
    }

    return false;
}

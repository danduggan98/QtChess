#include "piece.h"
#include <QDebug>

Piece::Piece(int x, int y, QString type, char color, QObject *parent) : QObject(parent) {
    x_pos_ = x;
    y_pos_ = y;
    type_ = type;
    color_ = color;
    real_moveset_ = {};
    potential_moveset_ = {};
    times_moved_ = 0;
}

//Move the piece to a new square
void Piece::ChangePos(Coord m) {
    x_pos_ = m.X();
    y_pos_ = m.Y();
    times_moved_++;
}

//See if a given coordinate is in our moveset, indicating it can be played
bool Piece::IsValidMove(Coord m) {

    //Check that the move is a real square
    if (m.isOnBoard()) {

        //Look for the move in our moveset
        for (unsigned int i = 0; i < real_moveset_.size(); i++) {
            if (real_moveset_[i] == m) {
                return true;
            }
        }
    }
    return false;
}

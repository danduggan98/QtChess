#include "piece.h"
#include <QDebug>

Piece::Piece(int x, int y, QString type, char color, QObject *parent) : QObject(parent) {
    x_pos_ = x;
    y_pos_ = y;
    type_ = type;
    color_ = color;
    real_moveset_ = {};
    potential_moveset_ = {};

    CreatePotentialMoveset();
}

//Move the piece to a new square
void Piece::ChangePos(Coord m) {
    x_pos_ = m.X();
    y_pos_ = m.Y();

    CreatePotentialMoveset(); //Update the potential moves
}

//Give this piece a list of moves it can make
void Piece::CreatePotentialMoveset() {
    potential_moveset_.clear();

    //Add the possible moveset for each piece
    //INCOMPLETE
    if (type_ == "pawn") {
        potential_moveset_.push_back(Coord(x_pos_ - 1, y_pos_ - 1));
        potential_moveset_.push_back(Coord(x_pos_, y_pos_ - 1));
        potential_moveset_.push_back(Coord(x_pos_, y_pos_ - 2));
        potential_moveset_.push_back(Coord(x_pos_ + 1, y_pos_ - 1));
    }
    //PLACEHOLDER
    else {
        potential_moveset_.push_back(Coord(x_pos_ - 1, y_pos_ - 1));
    }
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

#include "piece.h"

Piece::Piece(int x, int y, std::string type, QColor color, QObject *parent) : QObject(parent) {
    x_pos_ = x;
    y_pos_ = y;
    type_ = type;
    color_ = color;

    //Define moveset
}

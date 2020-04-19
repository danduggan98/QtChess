#include "board.h"

#include <QGraphicsScene>
#include <QDebug>

Square* Board::board_[Board::numRows][Board::numCols] = {};

//Constructor
Board::Board(QGraphicsScene *scene, QObject *parent) : QObject(parent) {
    board_scene_ = scene;

    width_ = board_scene_->width();
    height_ = board_scene_->width();

    square_width_ = width_ / (float) Board::numCols;
    square_height_ = height_ / (float) Board::numRows;

    //Add the grid and empty squares
    for (int i = 0; i < Board::numRows; i++) {
        for (int j = 0; j < Board::numCols; j++) {
            Square* s = new Square(i, j, square_width_, square_height_, true);
            board_[i][j] = s;
            board_scene_->addItem(s);
        }
    }

    //Place the pieces in their default state
    Reset();
}

//Move the pieces to their starting positions
void Board::Reset() {
    qDebug() << width_ << " " << height_;
}

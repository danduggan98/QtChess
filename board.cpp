#include "board.h"

#include <QGraphicsScene>
#include <QDebug>

Square* Board::board_[Board::numRows][Board::numCols] = {};

//Constructor
Board::Board(QGraphicsView *view, QObject *parent) : QObject(parent) {
    board_scene_ = view->scene();

    width_ = view->frameSize().width()-3;
    height_ = view->frameSize().height()-3;

    square_width_ = width_ / (float) Board::numCols;
    square_height_ = height_ / (float) Board::numRows;

    //Add the grid and empty squares
    QColor tan(225,210,180);
    QColor brown(175,120,65);
    QColor square_color;

    for (int i = 0; i < Board::numRows; i++) {
        for (int j = 0; j < Board::numCols; j++) {

            //Determine the square color
            if ((i % 2 == 1 && j % 2 == 0) || (i % 2 == 0 && j % 2 == 1)) {
                square_color = brown;
            }
            else {
                 square_color = tan;
            }

            //Add the square to both the board and the scene
            Square* s = new Square(i, j, square_width_, square_height_,
                                   true, square_color);
            board_[i][j] = s;
            board_scene_->addItem(s);
        }
    }

    //Place the pieces in their default state
    Reset();
}

//Move the pieces to their starting positions
void Board::Reset() {
    qDebug() << "Pieces set to their starting positions";
}

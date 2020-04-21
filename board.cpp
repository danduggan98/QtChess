#include "board.h"

#include <QGraphicsScene>
#include <QDebug>

Square* Board::board_[Board::numRows][Board::numCols] = {};

//Constructor
Board::Board(QGraphicsView *view, QObject *parent) : QObject(parent) {
    board_scene_ = view->scene();

    width_ = view->frameSize().width();
    height_ = view->frameSize().height();

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
            board_[j][i] = s;
            board_scene_->addItem(s);
        }
    }

    //Place the pieces in their default state
    Reset();
}

//Add a piece at its x and y positions on the board
void Board::AddPiece(Piece *piece) {
    int r = piece->get_x();
    int c = piece->get_y();
    board_[r][c]->SetPiece(piece);
}

//Move the pieces to their starting positions
void Board::Reset() {

    //Pawns
    for (int i = 0; i < Board::numCols; i++) {
        AddPiece(new Piece(1, i, "pawn", 'b')); //Black pawns
        AddPiece(new Piece(6, i, "pawn", 'w')); //White pawns
    }

    //Rooks
    AddPiece(new Piece(0, 0, "rook", 'b'));
    AddPiece(new Piece(0, 7, "rook", 'b'));
    AddPiece(new Piece(7, 0, "rook", 'w'));
    AddPiece(new Piece(7, 7, "rook", 'w'));

    //Knights
    AddPiece(new Piece(0, 1, "knight", 'b'));
    AddPiece(new Piece(0, 6, "knight", 'b'));
    AddPiece(new Piece(7, 1, "knight", 'w'));
    AddPiece(new Piece(7, 6, "knight", 'w'));

    //Bishops
    AddPiece(new Piece(0, 2, "bishop", 'b'));
    AddPiece(new Piece(0, 5, "bishop", 'b'));
    AddPiece(new Piece(7, 2, "bishop", 'w'));
    AddPiece(new Piece(7, 5, "bishop", 'w'));

    //Queens
    AddPiece(new Piece(0, 3, "queen", 'b'));
    AddPiece(new Piece(7, 3, "queen", 'w'));

    //Kings
    AddPiece(new Piece(0, 4, "king", 'b'));
    AddPiece(new Piece(7, 4, "king", 'w'));

    qDebug() << "Pieces set to their starting positions";
}

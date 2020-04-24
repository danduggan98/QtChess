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
            board_[i][j] = s;
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
    GetSquareAt(Coord(r, c))->SetPiece(piece);
}

//Move a piece from one coordinate to another
void Board::MovePiece(Coord from, Coord to) {
    Square* fromSquare = GetSquareAt(from);
    Square* toSquare = GetSquareAt(to);

    //Add the piece to the new square and remove it from the old one
    if (!fromSquare->isEmpty()) {
        toSquare->SetPiece(fromSquare->get_piece());
        fromSquare->get_piece()->ChangePos(to);
        fromSquare->RemovePiece();
    }
    else {
        qDebug() << "Square is empty. No piece to move.";
    }
}

//Move the pieces to their starting positions
void Board::Reset() {

    //Pawns
    for (int i = 0; i < Board::numCols; i++) {
        AddPiece(new Piece(i, 1, "pawn", 'b')); //Black pawns
        AddPiece(new Piece(i, 6, "pawn", 'w')); //White pawns
    }

    //Rooks
    AddPiece(new Piece(0, 0, "rook", 'b'));
    AddPiece(new Piece(7, 0, "rook", 'b'));
    AddPiece(new Piece(0, 7, "rook", 'w'));
    AddPiece(new Piece(7, 7, "rook", 'w'));

    //Knights
    AddPiece(new Piece(1, 0, "knight", 'b'));
    AddPiece(new Piece(6, 0, "knight", 'b'));
    AddPiece(new Piece(1, 7, "knight", 'w'));
    AddPiece(new Piece(6, 7, "knight", 'w'));

    //Bishops
    AddPiece(new Piece(2, 0, "bishop", 'b'));
    AddPiece(new Piece(5, 0, "bishop", 'b'));
    AddPiece(new Piece(2, 7, "bishop", 'w'));
    AddPiece(new Piece(5, 7, "bishop", 'w'));

    //Queens
    AddPiece(new Piece(3, 0, "queen", 'b'));
    AddPiece(new Piece(3, 7, "queen", 'w'));

    //Kings
    AddPiece(new Piece(4, 0, "king", 'b'));
    AddPiece(new Piece(4, 7, "king", 'w'));

    qDebug() << "Pieces set to their starting positions";
}

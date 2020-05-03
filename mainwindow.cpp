#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "board.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("QtChess");

    //Create a scene for the board and attach it to our ui
    QGraphicsView *board_graphics = ui->Board;
    board_scene = new QGraphicsScene;
    board_graphics->setScene(board_scene);

    //Create the board
    Board board(board_graphics);
    this->board_ptr = &board;

    //Connect each square to its slot
    for (int i = 0; i < Board::numRows; i++) {
        for (int j = 0; j < Board::numCols; j++) {
            connect(Board::board_[i][j], &Square::SquareSelected, this, &MainWindow::SquareSelectedSlot);
        }
    }
}

//Clear the highlights for any previously selected squares
void MainWindow::RemoveHighlights() {

    for (int i = 0; i < Board::numRows; i++) {
        for (int j = 0; j < Board::numCols; j++) {
            board_ptr->GetSquareAt(Coord(i, j))->RemoveHighlight();
        }
    }
}

//Highlight all squares in the piece's moveset
void MainWindow::HighlightMoves(Square *s) {

    std::vector<Coord> moves = s->get_piece()->get_moves();
    for (unsigned int k = 0; k < moves.size(); k++) {
        board_ptr->GetSquareAt(moves[k])->Highlight();
    }
}

//Remove previous selections
void MainWindow::RemoveSelections() {

    for (int i = 0; i < Board::numRows; i++) {
        for (int j = 0; j < Board::numCols; j++) {
            board_ptr->GetSquareAt(Coord(i, j))->Deselect();
        }
    }
}

//Take the kings out of check
void MainWindow::RemoveAttacks() {
    for (int i = 0; i < Board::numRows; i++) {
        for (int j = 0; j < Board::numCols; j++) {
            Piece* p = board_ptr->GetSquareAt(Coord(i, j))->get_piece();
            if (p) {
                p->ChangeAttackStatus(false);
            }
        }
    }
}

//Select a piece when it's clicked
void MainWindow::SquareSelectedSlot(Square *s) {
    Piece* nextPiece = s->get_piece();

    //Select the square if it contains a piece
    RemoveSelections();
    qDebug() << "Selected square (" << s->get_x() << ", " << s->get_y() << ")";

    if (nextPiece) {
        s->Select();
        qDebug() << "Square contains a " << nextPiece->get_color() << nextPiece->get_type();
    }
    else {
        qDebug() << "Square is empty";
    }

    //If a square was previously selected, move the piece from that square to this one if they are not allies
    if (lastSelectedSquare) {
        Piece* prevPiece = lastSelectedSquare->get_piece();
        Coord from = lastSelectedSquare->get_coords();
        Coord to = s->get_coords();

        //If they clicked the same piece twice, deselect it
        if (from == to) {
            s->Deselect();
            RemoveHighlights();
            lastSelectedSquare = nullptr;
        }

        //If they clicked on two consecutive ally pieces, just select the new one
        else if (nextPiece && (prevPiece->get_color() == nextPiece->get_color())) {
            lastSelectedSquare = s;
            RemoveHighlights();
            HighlightMoves(s);
        }

        //If they clicked two different squares, move the piece
        else {
            RemoveAttacks();
            board_ptr->MovePiece(from, to);
            RemoveHighlights();
            lastSelectedSquare = nullptr; //Reset the pointer so they can make the next move
        }
    }

    //No piece previously selected - select this one if it has a piece on it
    else if (nextPiece) {
        lastSelectedSquare = s;
        RemoveHighlights();
        HighlightMoves(s);
    }

    //No piece on this square - nothing selected
    else {
        lastSelectedSquare = nullptr;
        RemoveHighlights();
    }
    update();
}

MainWindow::~MainWindow() {
    delete ui;
}


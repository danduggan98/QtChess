#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "board.h"
#include <QtWidgets>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{


    ui->setupUi(this);
    this->setWindowTitle("QtChess");
    whose_turn = 'w'; //White always goes first

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
    Board::bKingPtr->ChangeAttackStatus(false);
    Board::wKingPtr->ChangeAttackStatus(false);
}

//Change who can move, along with the alerts indicating this
void MainWindow::GoToNextTurn() {
    if (whose_turn == 'w') {
        whose_turn = 'b';
        ui->BlackTurnLabel->setText("Black Turn");
        ui->WhiteTurnLabel->setText("");
    }
    else {
        whose_turn = 'w';
        ui->WhiteTurnLabel->setText("White Turn");
        ui->BlackTurnLabel->setText("");
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

        //If they clicked two different squares, move the piece and go to the next turn
        else {
            RemoveAttacks();
            board_ptr->MovePiece(from, to);
            RemoveHighlights();
            lastSelectedSquare = nullptr; //Reset the pointer so they can make the next move

            //If the move succeeded, go to the next turn
            if (board_ptr->GetSquareAt(from)->isEmpty()) {
                GoToNextTurn();
            }
        }
    }

    //No piece previously selected - select this one if it has a piece and it's that player's turn
    else if (nextPiece) {
        if (nextPiece->get_color() != whose_turn) {
            qDebug() << "Can not move that piece. It is" << (whose_turn == 'w' ? "white's" : "black's") << "turn";
            return;
        }
        else {
            lastSelectedSquare = s;
            RemoveHighlights();
            HighlightMoves(s);
        }
    }

    //No piece on this square - nothing selected
    else {
        lastSelectedSquare = nullptr;
        RemoveHighlights();
    }
    update();
}

void MainWindow::PieceSelectedSlot(Piece *piece){
    //will call the level up function
    levelUp(piece);

}

void MainWindow::levelUp(Piece *piece){
    if(Board::wCapturedPieces.size() == 5){
        qDebug()<< "Congratulations! You've reached your first Level Up!";
        qDebug()<< "Now, please click on a piece that you have captured, to bring back to the board, and it play for you're team.";
        levelUpUpdate(piece);
    }else if(Board::bCapturedPieces.size() == 5){
        qDebug()<< "Congratulations! You've reached your first Level Up!";
        qDebug()<< "Now, please click on a piece that you have captured, to bring back to the board, and it play for you're team.";
        levelUpUpdate(piece);
    }else{

    }
}

void MainWindow::levelUpUpdate(Piece *piece){
    if(board_ptr->GetSquareAt(piece->get_coords())->isEmpty()){
        board_ptr->AddPiece(piece);
    }else{
        Coord lvlUpX = piece->get_coords();
        Coord lvlUpY = piece->get_coords();

        //Coord newCoord = piece->get_coords();
        Coord newCoord = Coord(lvlUpX.X(), lvlUpY.Y());
        piece->ChangePos(newCoord);
        board_ptr->AddPiece(piece);
    }
}

/*
void MainWindow::addGraphicsToBar(Piece* p){
    //import graphics when piece has been capture
    //show piece in bar


    ui->setupUi(this);
    ui->BlackCapturedPieces;
    ui->WhiteCapturedPieces;
}
*/

MainWindow::~MainWindow() {
    delete ui;
}

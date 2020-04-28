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
    GetSquareAt(piece->get_coords())->SetPiece(piece);
    DefinePotentialMoveset(piece);
    DefineMoveset(piece);
}

//Move a piece from one coordinate to another
void Board::MovePiece(Coord from, Coord to) {
    Square* fromSquare = GetSquareAt(from);
    Square* toSquare = GetSquareAt(to);

    //Check that they selected a piece
    if (!fromSquare->isEmpty()) {

        //Check that they selected two different squares
        if(fromSquare != toSquare) {

            //Check that the move is valid
            if (fromSquare->get_piece()->IsValidMove(to)) {

                //Add the piece to the new square and remove it from the old one
                toSquare->SetPiece(fromSquare->get_piece());
                fromSquare->get_piece()->ChangePos(to);
                fromSquare->RemovePiece();

                //Update the real and potential moves for each piece now that positions have changed
                UpdateMovesets();
            }
        }
        else {
            qDebug() << "Selected same piece twice. No action taken.";
        }
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

//Determine the valid moves for a piece, and pass them to the piece itself
void Board::DefineMoveset(Piece* p) {
    std::vector<Coord> new_moveset;

    std::vector<Coord> potential_moveset = p->get_potential_moves();
    Coord p_coords = p->get_coords();

    //Iterate through the theoretical moves and see which are possible in this position
    for (unsigned int i = 0; i < potential_moveset.size(); i++) {
        Coord potential_move = potential_moveset[i];

        if (potential_move.isOnBoard()) {
            if (!ContainsAlly(p_coords, potential_move)) {
                new_moveset.push_back(potential_move);
            }
        }
    }
    p->SetMoveset(new_moveset);
}

//Determine the potential moves for a piece and pass them to the piece itself
void Board::DefinePotentialMoveset(Piece* p) {
    std::vector<Coord> potential_moveset;

    int dir = (p->get_color() == 'w') ? 1 : -1; //Sign indicates direction of moves - positive (up) for white and negative (down) for black
    int x = p->get_x();
    int y = p->get_y();
    Coord cur = p->get_coords();
    int times_moved = p->get_times_moved();
    QString type = p->get_type();

    //Add the possible moveset for each piece
    //INCOMPLETE
    //DOES NOT TAKE INTO ACCOUNT MOVES THAT ARE "BLOCKED" BY OTHER PIECES, ALLY OR ENEMY
    // FIX - MERGE MOVESET DEF FUNCTIONS TOGETHER - START CHECKIGN IN EACH DIRECITON AND STOP WHEN YOU HIT ANOTHER PIECE
    // IF THAT'S AN ENEMY, INCLUDE IT IN THE MOVESET, ELSE JUST STOP
    if (type == "pawn") {

        //One square forward
        Coord oneFwd(x, y - dir);
        if (oneFwd.isOnBoard() && !ContainsPiece(oneFwd)) {
            potential_moveset.push_back(oneFwd);
        }

        //Two squares forward - only on first turn
        Coord twoFwd(x, y - (2*dir));
        if (twoFwd.isOnBoard() && times_moved == 0 && !ContainsPiece(oneFwd) && !ContainsPiece(twoFwd) ) {
            potential_moveset.push_back(twoFwd);
        }

        //Capture an enemy to the left
        Coord leftCapture(x - dir, y - dir);
        if (leftCapture.isOnBoard() && ContainsEnemy(cur, leftCapture)) {
            potential_moveset.push_back(leftCapture);
        }

        //Capture an enemy to the right
        Coord rightCapture(x + dir, y - dir);
        if (rightCapture.isOnBoard() && ContainsEnemy(cur, rightCapture)) {
            potential_moveset.push_back(rightCapture);
        }
    }
    else if (type == "rook") {
        for (int i = x - numCols; i < x + numCols; i++) { //Horizontal
            potential_moveset.push_back(Coord(i, y));
        }
        for (int i = y - numRows; i < y + numRows; i++) { //Vertical
            potential_moveset.push_back(Coord(x, i));
        }
    }
    else if (type == "bishop") {
        for (int i = x - numCols, j = y - numRows; i < x + numCols; i++, j++) { //Diagonal from top left to bottom right
            potential_moveset.push_back(Coord(i, j));
        }
        for (int i = x - numCols, j = y + numRows; i < x + numCols; i++, j--) { //Diagonal from bottom left to top right
            potential_moveset.push_back(Coord(i, j));
        }
    }
    else if (type == "knight") {
        potential_moveset.push_back(Coord(x-2, y-1)); //Left
        potential_moveset.push_back(Coord(x-2, y+1));
        potential_moveset.push_back(Coord(x-1, y+2)); //Up
        potential_moveset.push_back(Coord(x+1, y+2));
        potential_moveset.push_back(Coord(x+2, y-1)); //Right
        potential_moveset.push_back(Coord(x+2, y+1));
        potential_moveset.push_back(Coord(x-1, y-2)); //Down
        potential_moveset.push_back(Coord(x+1, y-2));
    }
    else if (type == "queen") {
        for (int i = x - numCols; i < x + numCols; i++) { //Horizontal
            potential_moveset.push_back(Coord(i, y));
        }
        for (int i = y - numRows; i < y + numRows; i++) { //Vertical
            potential_moveset.push_back(Coord(x, i));
        }
        for (int i = x - numCols, j = y - numRows; i < x + numCols; i++, j++) { //Diagonal from top left to bottom right
            potential_moveset.push_back(Coord(i, j));
        }
        for (int i = x - numCols, j = y + numRows; i < x + numCols; i++, j--) { //Diagonal from bottom left to top right
            potential_moveset.push_back(Coord(i, j));
        }
    }
    else if (type == "king") {
        potential_moveset.push_back(Coord(x-1, y)); //Sides
        potential_moveset.push_back(Coord(x+1, y));
        potential_moveset.push_back(Coord(x-1, y-1)); //Forward
        potential_moveset.push_back(Coord(x+1, y-1));
        potential_moveset.push_back(Coord(x, y-1));
        potential_moveset.push_back(Coord(x-1, y+1)); //Backward
        potential_moveset.push_back(Coord(x+1, y+1));
        potential_moveset.push_back(Coord(x, y+1));
    }

    p->SetPotentialMoveset(potential_moveset);
}

//Define new movesets for every piece
void Board::UpdateMovesets() {
    for (int i = 0; i < numRows; i++) {
        for (int j = 0; j < numCols; j++) {
            Coord pos(i,j);
            Piece* curPiece = GetSquareAt(pos)->get_piece();

            if (curPiece) {
                DefinePotentialMoveset(curPiece);
                DefineMoveset(curPiece);
            }
        }
    }
}

//See if a coordinate contains a piece of the same color
bool Board::ContainsAlly(Coord c1, Coord c2) {

    Piece* p1 = GetSquareAt(c1)->get_piece();
    Piece* p2 = GetSquareAt(c2)->get_piece();

    //See if both squares have pieces
    if (p1 && p2) {
        if (p1->get_color() == p2->get_color()) {
            return true;
        }
    }
    return false;
}

//See if a coordinate contains a piece of a different color
bool Board::ContainsEnemy(Coord c1, Coord c2) {

    Piece* p1 = GetSquareAt(c1)->get_piece();
    Piece* p2 = GetSquareAt(c2)->get_piece();

    //See if both squares have pieces
    if (p1 && p2) {
        if (p1->get_color() != p2->get_color()) {
            return true;
        }
    }
    return false;
}

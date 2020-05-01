#include "board.h"

#include <QGraphicsScene>
#include <QDebug>

Square* Board::board_[Board::numRows][Board::numCols] = {};
Coord Board::wKingPos(4,7);
Coord Board::bKingPos(4,0);

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

//Move a piece from one coordinate to another
void Board::MovePiece(Coord from, Coord to) {
    Square* fromSquare = GetSquareAt(from);
    Square* toSquare = GetSquareAt(to);

    //Check that they selected a piece
    if (!fromSquare->isEmpty()) {

        //Check that they selected two different squares
        if(fromSquare != toSquare) {

            //Check that the move is valid
            Piece* movingPiece = fromSquare->get_piece();
            if (movingPiece->IsValidMove(to)) {

                //Add the piece to the new square and remove it from the old one
                movingPiece->ChangePos(to);
                toSquare->SetPiece(movingPiece);
                fromSquare->RemovePiece();

                //If a king is moved, update its position
                if (ContainsKing(to)) {
                    qDebug() << "MOVED THE KING";
                    movingPiece->get_color() == 'w' ? wKingPos = to : bKingPos = to;
                }

                //Update the moves for each piece now that positions have changed
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
    UpdateMovesets();
}

//Check if a single move is valid, add it if so (Used only by knight)
void Board::AddMove(Coord startPos, Coord endPos, std::vector<Coord> &temp_moveset) {
    if (endPos.isOnBoard() && !ContainsAlly(startPos, endPos)) {
        if (ContainsKing(endPos)) {
            qDebug() << "KING IS IN CHECK!!!!!";
        }
        temp_moveset.push_back(endPos);
    }
}

//Check for valid moves at all squares in between two coordinates - includes blocking
void Board::AddMovespace(Coord startPos, Coord endPos, std::vector<Coord> &temp_moveset) {

    int x = startPos.X();
    int y = startPos.Y();
    int endX = endPos.X();
    int endY = endPos.Y();

    int deltaX = endX - x;
    int deltaY = endY - y;

    int xAdjust = deltaX == 0 ? 0 : (deltaX > 0 ? 1 : -1); //Zero or sign of delta x
    int yAdjust = deltaY == 0 ? 0 : (deltaY > 0 ? 1 : -1); //Zero or sign of delta y

    //Go until both axes reach their end squares
    while (x != endX + xAdjust || y != endY + yAdjust) {
        Coord newPos(x, y);

        if (newPos != startPos) {                      //Don't add the current square
            if (newPos.isOnBoard()) {                  //Don't add any invalid coordinates
                if (GetSquareAt(newPos)->isEmpty()) {  //Empty square - add it
                    temp_moveset.push_back(newPos);
                }
                else {
                    if (ContainsAlly(startPos, newPos)) { break; }  // Ally in the way - block everything including and beyond it
                    temp_moveset.push_back(newPos);
                    if (ContainsEnemy(startPos, newPos)) { break; } // Enemy in the way - add the enemy (previous line) but block everything beyond it
                }
            }
            else {
                break;
            }
        }
        x += xAdjust;
        y += yAdjust;
    }
}

//Determine the valid moves for a piece and pass them to the piece itself
void Board::DefineMoveset(Piece* p) {
    std::vector<Coord> new_moveset;

    int x = p->get_x();
    int y = p->get_y();
    Coord cur = p->get_coords();
    int times_moved = p->get_times_moved();
    QString type = p->get_type();

    //PAWNS
    if (type == "pawn") {
        int dir = (p->get_color() == 'w') ? 1 : -1; //Sign indicates direction of pawn moves - positive (up) for white and negative (down) for black

        //One square forward
        Coord oneFwd(x, y - dir);
        if (oneFwd.isOnBoard() && !ContainsPiece(oneFwd)) {
            new_moveset.push_back(oneFwd);
        }

        //Two squares forward - only on first turn
        Coord twoFwd(x, y - (2*dir));
        if (twoFwd.isOnBoard() && times_moved == 0 && !ContainsPiece(oneFwd) && !ContainsPiece(twoFwd) ) {
            new_moveset.push_back(twoFwd);
        }

        //Capture an enemy to the left
        Coord leftCapture(x - dir, y - dir);
        if (leftCapture.isOnBoard() && ContainsEnemy(cur, leftCapture)) {
            new_moveset.push_back(leftCapture);
        }

        //Capture an enemy to the right
        Coord rightCapture(x + dir, y - dir);
        if (rightCapture.isOnBoard() && ContainsEnemy(cur, rightCapture)) {
            new_moveset.push_back(rightCapture);
        }
    }

    //ROOKS
    else if (type == "rook") {
        AddMovespace(cur, Coord(0, y), new_moveset);              //Left horizontal
        AddMovespace(cur, Coord(Board::numCols, y), new_moveset); //Right horizontal
        AddMovespace(cur, Coord(x, 0), new_moveset);              //Upper vertical
        AddMovespace(cur, Coord(x, Board::numRows), new_moveset); //Lower vertical
    }

    //BISHOPS
    else if (type == "bishop") {
        AddMovespace(cur, Coord(0, 0), new_moveset);                           //Upper left diagonal
        AddMovespace(cur, Coord(Board::numCols, 0), new_moveset);              //Upper right diagonal
        AddMovespace(cur, Coord(0, Board::numRows), new_moveset);              //Lower left diagonal
        AddMovespace(cur, Coord(Board::numCols, Board::numRows), new_moveset); //Lower right diagonal
    }

    //KNIGHTS
    else if (type == "knight") {
        AddMove(cur, Coord(x - 2, y - 1), new_moveset); //Left
        AddMove(cur, Coord(x - 2, y + 1), new_moveset);
        AddMove(cur, Coord(x - 1, y + 2), new_moveset); //Up
        AddMove(cur, Coord(x + 1, y + 2), new_moveset);
        AddMove(cur, Coord(x + 2, y - 1), new_moveset); //Right
        AddMove(cur, Coord(x + 2, y + 1), new_moveset);
        AddMove(cur, Coord(x - 1, y - 2), new_moveset); //Down
        AddMove(cur, Coord(x + 1, y - 2), new_moveset);
    }

    //QUEENS
    else if (type == "queen") {
        AddMovespace(cur, Coord(0, y), new_moveset);                           //Left horizontal
        AddMovespace(cur, Coord(Board::numCols, y), new_moveset);              //Right horizontal
        AddMovespace(cur, Coord(x, 0), new_moveset);                           //Upper vertical
        AddMovespace(cur, Coord(x, Board::numRows), new_moveset);              //Lower vertical

        AddMovespace(cur, Coord(0, 0), new_moveset);                           //Upper left diagonal
        AddMovespace(cur, Coord(Board::numCols, 0), new_moveset);              //Upper right diagonal
        AddMovespace(cur, Coord(0, Board::numRows), new_moveset);              //Lower left diagonal
        AddMovespace(cur, Coord(Board::numCols, Board::numRows), new_moveset); //Lower right diagonal
    }

    //KINGS
    else if (type == "king") {
        AddMove(cur, Coord(x - 1, y), new_moveset);     //Sides
        AddMove(cur, Coord(x + 1, y), new_moveset);
        AddMove(cur, Coord(x - 1, y - 1), new_moveset); //Forward
        AddMove(cur, Coord(x + 1, y - 1), new_moveset);
        AddMove(cur, Coord(x, y - 1), new_moveset);
        AddMove(cur, Coord(x - 1, y + 1), new_moveset); //Backward
        AddMove(cur, Coord(x + 1, y + 1), new_moveset);
        AddMove(cur, Coord(x, y + 1), new_moveset);
    }

    p->SetMoveset(new_moveset); //Store the updated moveset in the piece
}

//Define new movesets for every piece
void Board::UpdateMovesets() {
    for (int i = 0; i < numRows; i++) {
        for (int j = 0; j < numCols; j++) {
            Coord pos(i,j);
            Piece* curPiece = GetSquareAt(pos)->get_piece();

            if (curPiece) {
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

//Check if the coordinate contains a king
bool Board::ContainsKing(Coord c) {
    Piece* p = GetSquareAt(c)->get_piece();

    if (p) {
        return (p->get_type() == "king");
    }
    return false;
}

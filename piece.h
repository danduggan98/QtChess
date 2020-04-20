#ifndef PIECE_H
#define PIECE_H

#include "move.h"

#include <QObject>
#include <QColor>
#include <QString>
#include <vector>

class Piece : public QObject {
    Q_OBJECT

    private:
        int x_pos_;
        int y_pos_;
        QString type_;
        char color_;
        std::vector<Move> moveset_();

    public:
        explicit Piece(int x, int y, QString type, char color, QObject *parent = nullptr);
        QString get_type() { return type_; };
        void DefineMoveset();
        void MovePiece(Move m);
        bool isValidMove(Move m);

    signals:

};

#endif // PIECE_H

#ifndef PIECE_H
#define PIECE_H

#include "coord.h"

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
        std::vector<Coord> moveset_;

    public:
        explicit Piece(int x, int y, QString type, char color, QObject *parent = nullptr);
        int get_x() { return x_pos_; };
        int get_y() { return y_pos_; };
        QString get_type() { return type_; };
        char get_color() { return color_; };

        void DefineMoveset();
        void ChangePos(Coord m);
        bool isValidMove(Coord m);
};

#endif // PIECE_H

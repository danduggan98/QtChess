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
        int times_moved_;

    public:
        explicit Piece(int x, int y, QString type, char color, QObject *parent = nullptr);
        int get_x() { return x_pos_; };
        int get_y() { return y_pos_; };
        Coord get_coords() { return Coord(x_pos_, y_pos_); };
        QString get_type() { return type_; };
        char get_color() { return color_; };
        int get_times_moved() { return times_moved_; };
        std::vector<Coord> get_moves() { return moveset_; };

        void ChangePos(Coord m);
        void SetMoveset(std::vector<Coord> new_moveset) { moveset_ = new_moveset; };
        bool IsValidMove(Coord m);
};

#endif // PIECE_H

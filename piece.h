#ifndef PIECE_H
#define PIECE_H

#include <QObject>
#include <QColor>
#include <string>

class Piece : public QObject {
    Q_OBJECT

    private:
        int x_pos_;
        int y_pos_;
        std::string type_;
        QColor color_;

    public:
        explicit Piece(int x, int y, std::string type, QColor color, QObject *parent = nullptr);
        void DefineMoveset();
        void Move(int new_x, int new_y);

    signals:

};

#endif // PIECE_H

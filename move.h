#ifndef MOVE_H
#define MOVE_H

class Move {
    private:
        int x_;
        int y_;

    public:
        Move(int x, int y) {
            x_ = x;
            y_ = y;
        }
        int X() { return x_; };
        int Y() { return y_; };
};

#endif // MOVE_H

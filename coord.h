#ifndef COORD_H
#define COORD_H

class Coord {
    private:
        int x_;
        int y_;

    public:
        Coord(int x, int y) {
            x_ = x;
            y_ = y;
        }
        int X() { return x_; };
        int Y() { return y_; };
};

#endif // COORD_H
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

        //Check if coordinate points to a real square
        bool isOnBoard() {
            return (x_ >= 0 && x_ < 8 && y_ >= 0 && y_ < 8);
        }

        //Compare Coords
        friend bool operator==(Coord& first, Coord& second) {
            return (first.X() == second.X() && first.Y() == second.Y());
        }
};

#endif // COORD_H

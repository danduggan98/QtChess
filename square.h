#ifndef SQUARE_H
#define SQUARE_H

#include <QObject>
#include <QGraphicsItem>

class Square : public QObject, public QGraphicsItem {
    Q_OBJECT

    private:
        int x_val_;
        int y_val_;
        float width_;
        float height_;
        bool empty_;
        /*Piece piece*/

    public:
        explicit Square(int x, int y, bool empty, /*Piece piece, */
                        QObject *parent = nullptr);

        int get_x() { return x_val_; };
        int get_y() { return y_val_; };
        bool isEmpty() { return empty_; };
        void setContents(/*Piece piece*/);

        QRectF boundingRect() const override;
        QPainterPath shape() const override;

    signals:
        void SquareSelected(Square *s);

    protected:
        void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
};

#endif // SQUARE_H

#ifndef SQUARE_H
#define SQUARE_H

#include <QObject>
#include <QGraphicsItem>
#

class Square : public QObject, public QGraphicsItem {
    Q_OBJECT

    private:
        int x_val_;
        int y_val_;
        float width_;
        float height_;
        bool empty_;
        QColor color_;
        /*Piece piece_*/

    public:
        explicit Square(int x, int y, float width, float height,
                        bool empty, QColor color, QObject *parent = nullptr); /*Piece piece,*/

        int get_x() { return x_val_; };
        int get_y() { return y_val_; };
        bool isEmpty() { return empty_; };
        /*Piece get_piece();*/
        /*void SetPiece(Piece piece);*/

        QRectF boundingRect() const override;
        QPainterPath shape() const override;
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *item,
                   QWidget *widget) override;

    protected:
        void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

    signals:
        void SquareSelected(Square *s);
};

#endif // SQUARE_H

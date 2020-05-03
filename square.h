#ifndef SQUARE_H
#define SQUARE_H

#include <QObject>
#include <QGraphicsItem>
#include "piece.h"

class Square : public QObject, public QGraphicsItem {
    Q_OBJECT

    private:
        int x_val_;
        int y_val_;
        float width_;
        float height_;
        bool empty_;
        QColor color_;
        Piece* piece_;
        bool is_highlighted_;
        bool is_selected_;

    public:
        explicit Square(int x, int y, float width, float height,
                        bool empty, QColor color, QObject *parent = nullptr);

        int get_x() { return x_val_; };
        int get_y() { return y_val_; };
        Coord get_coords() { return Coord(x_val_, y_val_); };
        bool isEmpty() { return empty_; };
        Piece* get_piece() { return piece_; };

        void Highlight() { is_highlighted_ = true; };
        void RemoveHighlight() { is_highlighted_ = false; };
        void Select() { is_selected_ = true; };
        void Deselect() { is_selected_ = false; };

        void RemovePiece();
        void SetPiece(Piece* piece);

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

#include "square.h"

#include <QtWidgets>

Square::Square(int x, int y, bool empty, QObject *parent) : QObject(parent) {
    x_val_ = x;
    y_val_ = y;
    empty_ = empty;
}

//Set the square's boundaries
QRectF Square::boundingRect() const {
    return QRectF(x_val_ * this->width_, y_val_ * this->height_, width_, height_);
}

//Square coordinates
QPainterPath Square::shape() const {
    QPainterPath path;
    path.addRect(x_val_ * this->width_, y_val_ * this->height_, width_, height_);
    return path;
}

//Respond when the square is clicked
void Square::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        emit SquareSelected(this);
    }
}

#include "square.h"

#include <QtWidgets>

Square::Square(int x, int y, float width, float height,
               bool empty, QObject *parent) : QObject(parent) {
    x_val_ = x;
    y_val_ = y;
    width_ = width;
    height_ = height;
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

void Square::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(widget);
    Q_UNUSED(option);

    QBrush b = painter->brush();
    painter->setBrush(QBrush(QColor(0,0,0))); //White
    painter->drawRect(QRect(this->x_val_ * this->width_, this->y_val_ * this->height_, this->width_, this->height_));
    painter->setBrush(b);
}

//Respond when the square is clicked
void Square::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        emit SquareSelected(this);
    }
}

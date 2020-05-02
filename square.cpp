#include "square.h"

#include <QtWidgets>
#include <QDebug>

Square::Square(int x, int y, float width, float height,
               bool empty, QColor color, QObject *parent) : QObject(parent) {
    x_val_ = x;
    y_val_ = y;
    width_ = width;
    height_ = height;
    empty_ = empty;
    color_ = color;
    piece_ = nullptr;
    isHighlighted = false;
    isSelected = false;
}

void Square::RemovePiece() {
    piece_ = nullptr;
    empty_ = true;
}

//Add a piece to this square
void Square::SetPiece(Piece *piece) {
    piece_ = piece;
    empty_ = false;
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

    //Remove black borders and tone down the color a bit
    painter->setPen(Qt::NoPen);
    painter->setOpacity(0.8);

    //Draw the square
    QBrush b = painter->brush();
    painter->setBrush(QBrush(color_));
    painter->drawRect(QRect(this->x_val_ * this->width_, this->y_val_ * this->height_, this->width_, this->height_));
    painter->setBrush(b);

    //Highlight the square if it contains a valid move for the current piece
    if (isHighlighted) {

        //Small blue dot if empty
        if (isEmpty()) {
            painter->setBrush(QBrush(QColor(0,0,255)));
            painter->setOpacity(0.35);
            painter->drawEllipse(QPoint(int((this->x_val_ * this->width_)+this->width_/2), int((this->y_val_ * this->height_)+this->height_/2)), int(this->width_/7), int(this->height_/7));
            painter->setBrush(b);
        }

        //Large blue square if occupied
        else {
            painter->setBrush(QBrush(QColor(0,0,255)));
            painter->setOpacity(0.25);
            painter->drawRect(QRect(this->x_val_ * this->width_, this->y_val_ * this->height_, this->width_, this->height_));
            painter->setBrush(b);
        }
    }

    //Color the square green if it's selected
    if (isSelected) {
        painter->setBrush(QBrush(QColor(0,200,0)));
        painter->setOpacity(0.25);
        painter->drawRect(QRect(this->x_val_ * this->width_, this->y_val_ * this->height_, this->width_, this->height_));
        painter->setBrush(b);
    }

    //Draw the piece icon if there is one
    if (!this->isEmpty()) {
        painter->setOpacity(1.0);
        float x = width_ * x_val_ + 2;
        float y = height_ * y_val_ - 1;

        //Construct the image file path
        QString filepath = ":/";
        filepath += QString(piece_->get_color()) + "_";
        filepath += QString(piece_->get_type()) + ".png";

        //Paint the image as a pixmap
        QPixmap pic(filepath);
        pic = pic.scaled(width_, height_, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        painter->drawPixmap(x, y, pic);
    }
}

//Respond when the square is clicked
void Square::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        emit SquareSelected(this);
    }
}

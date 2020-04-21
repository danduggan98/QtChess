#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "board.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //Create a scene for the board and attach it to our ui
    QGraphicsView *board_graphics = ui->Board;
    board_scene = new QGraphicsScene;
    board_graphics->setScene(board_scene);

    //Create the board
    Board board(board_graphics);
    this->board_ptr = &board;

    //Connect each square to its slot
    for (int i = 0; i < Board::numRows; i++) {
        for (int j = 0; j < Board::numCols; j++) {
            connect(Board::board_[i][j], &Square::SquareSelected, this, &MainWindow::SquareSelectedSlot);
        }
    }
}

void MainWindow::SquareSelectedSlot(Square *s) {
    qDebug() << "Selected square (" << s->get_x() << ", " << s->get_y() << ")";

    if (!s->isEmpty()) {
        qDebug() << "Square contains a " << s->get_piece()->get_color() << s->get_piece()->get_type();
    } else {
        qDebug() << "Square is empty";
    }
}

MainWindow::~MainWindow() {
    delete ui;
}


#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "board.h"

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

    public:
        MainWindow(QWidget *parent = nullptr);
        void RemoveHighlights();
        void HighlightMoves(Square *s);
        void SelectSquare(Square *s) { s->Select(); };
        void RemoveSelections();
        ~MainWindow();

    private:
        Board* board_ptr;
        Square* lastSelectedSquare = nullptr;

        Ui::MainWindow *ui;
        QGraphicsScene *board_scene;

    private slots:
        void SquareSelectedSlot(Square *s);
};
#endif // MAINWINDOW_H

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
        void RemoveSelections();
        void RemoveAttacks();
        void GoToNextTurn();
        ~MainWindow();

    private:
        Board* board_ptr;
        Square* lastSelectedSquare = nullptr;
        char whose_turn;

        Ui::MainWindow *ui;
        QGraphicsScene *board_scene;

    private slots:
        void SquareSelectedSlot(Square *s);
        void RestartButtonPushed();
};
#endif // MAINWINDOW_H

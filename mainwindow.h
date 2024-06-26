#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QGridLayout>
#include <QPushButton>
#include <vector>
#include <QToolButton>
#include <QTimer>
#include <QTime>
#include <QLabel>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onSolveClicked();
    void onResetClicked();
    void onGenerateClicked();
    void onStartClicked();
    void onVerifyClicked();
    void updateTimer();

private:
    Ui::MainWindow *ui;
    std::vector<std::vector<QLineEdit*>> cells;
    std::vector<std::vector<QToolButton*>> hintButtons; // Vector to hold hint buttons for each cell
    QPushButton *solveButton;
    QPushButton *resetButton;
    QPushButton *generateButton;
    QPushButton *startButton; // Button to start the game
    QPushButton *verifyButton; // Button to verify the solution
    QWidget *initialScreenWidget; // Widget for the initial screen
    QWidget *sudokuWidget; // Widget for the Sudoku interface
    QLabel *timerLabel;
    QTimer *timer;
    QTime time;

    void loadBoard(std::vector<std::vector<int>>& board);
    void displayBoard(const std::vector<std::vector<int>>& board);
    bool solveSudoku(std::vector<std::vector<int>>& board);
    bool isValid(const std::vector<std::vector<int>>& board, int row, int col, int num);
    bool findEmptyCell(std::vector<std::vector<int>>& board, int &row, int &col);
    bool checkSolution(const std::vector<std::vector<int>>& board);
    bool isSolutionCorrect(const std::vector<std::vector<int>>& board);
    const std::vector<std::vector<int>> exampleBoard = {
        {5, 3, 0, 0, 7, 0, 0, 0, 0},
        {6, 0, 0, 1, 9, 5, 0, 0, 0},
        {0, 9, 8, 0, 0, 0, 0, 6, 0},
        {8, 0, 0, 0, 6, 0, 0, 0, 3},
        {4, 0, 0, 8, 0, 3, 0, 0, 1},
        {7, 0, 0, 0, 2, 0, 0, 0, 6},
        {0, 6, 0, 0, 0, 0, 2, 8, 0},
        {0, 0, 0, 4, 1, 9, 0, 0, 5},
        {0, 0, 0, 0, 8, 0, 0, 7, 9}
    };
};

#endif // MAINWINDOW_H

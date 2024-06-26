#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Graph.h"
#include <vector>
#include <unordered_set>
#include <QDebug>
#include <QMessageBox>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPalette>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), time(0, 0) // Initialize time here
{
    ui->setupUi(this);

    // Create the initial screen
    initialScreenWidget = new QWidget(this);
    QVBoxLayout *initialLayout = new QVBoxLayout;

    QLabel *titleLabel = new QLabel("Let's Play Sudoku!", this);
    titleLabel->setFont(QFont("Arial", 24, QFont::Bold));
    titleLabel->setAlignment(Qt::AlignCenter);

    QLabel *instructionsLabel = new QLabel("Instructions:\n\n1. To solve a puzzle, fill in the grid with your puzzle and click 'Solve'.\n2. To reset the grid, click 'Reset'.\n3. To generate a new puzzle, click 'Generate'.\n\nClick 'START' to begin.", this);
    instructionsLabel->setAlignment(Qt::AlignLeft);
    instructionsLabel->setWordWrap(false);
    instructionsLabel->setFont(QFont("Arial", 14));
    instructionsLabel->setMargin(20);

    startButton = new QPushButton("START", this);
    startButton->setFixedSize(200, 50);
    startButton->setFont(QFont("Arial", 16));
    startButton->setStyleSheet("QPushButton { background-color: #4CAF50; color: white; border-radius: 10px; }"
                               "QPushButton:hover { background-color: #45a049; }");

    initialLayout->addStretch();
    initialLayout->addWidget(titleLabel);
    initialLayout->addWidget(instructionsLabel);
    initialLayout->addWidget(startButton);
    initialLayout->addStretch();
    initialLayout->setAlignment(Qt::AlignCenter);
    initialLayout->setSpacing(20);

    initialScreenWidget->setLayout(initialLayout);
    setCentralWidget(initialScreenWidget);

    // Connect the START button to its slot
    connect(startButton, &QPushButton::clicked, this, &MainWindow::onStartClicked);

    // Create the Sudoku interface but don't set it as the central widget yet
    sudokuWidget = new QWidget(this);

    QLabel *sudokuTitleLabel = new QLabel("Sudoku", this);
    sudokuTitleLabel->setFont(QFont("Arial", 24, QFont::Bold));
    sudokuTitleLabel->setAlignment(Qt::AlignCenter);

    QGridLayout *mainLayout = new QGridLayout;

    for (int i = 0; i < 9; ++i) {
        std::vector<QLineEdit*> row;
        for (int j = 0; j < 9; ++j) {
            QLineEdit *cell = new QLineEdit(this);
            cell->setFixedSize(35, 35);
            cell->setAlignment(Qt::AlignCenter);
            cell->setMaxLength(1);
            cell->setFont(QFont("Arial", 14));
            cell->setStyleSheet("QLineEdit { background-color: white; border: 1px solid gray; }");

            // Apply colored grid background for 3x3 sub-grids
            if ((i < 3 || i >= 6) && (j < 3 || j >= 6)) {
                cell->setStyleSheet("QLineEdit { background-color: #aeadac; border: 1px solid gray; }");
            } else if ((i >= 3 && i < 6) && (j >= 3 && j < 6)) {
                cell->setStyleSheet("QLineEdit { background-color: #aeadac; border: 1px solid gray; }");
            }

            row.push_back(cell);
            mainLayout->addWidget(cell, i, j);
        }
        cells.push_back(row);
    }

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    solveButton = new QPushButton("Solve", this);
    resetButton = new QPushButton("Reset", this);
    generateButton = new QPushButton("Generate", this);
    verifyButton = new QPushButton("Verify", this); // Add the Verify button
    solveButton->setFixedSize(100, 40);
    resetButton->setFixedSize(100, 40);
    generateButton->setFixedSize(100, 40);
    verifyButton->setFixedSize(100, 40); // Set size for the Verify button
    buttonLayout->addWidget(solveButton);
    buttonLayout->addWidget(resetButton);
    buttonLayout->addWidget(generateButton);
    buttonLayout->addWidget(verifyButton); // Add Verify button to the layout
    buttonLayout->setAlignment(Qt::AlignCenter);

    QVBoxLayout *verticalLayout = new QVBoxLayout;
    verticalLayout->addWidget(sudokuTitleLabel);

    // Add the timer label to the top-right corner
    QHBoxLayout *topLayout = new QHBoxLayout;
    topLayout->addWidget(sudokuTitleLabel);
    timerLabel = new QLabel("00:00", this);
    timerLabel->setFont(QFont("Arial", 16));
    timerLabel->setAlignment(Qt::AlignRight);
    topLayout->addWidget(timerLabel, 1, Qt::AlignRight);

    verticalLayout->addLayout(topLayout);
    verticalLayout->addLayout(mainLayout);
    verticalLayout->addLayout(buttonLayout);

    sudokuWidget->setLayout(verticalLayout);

    // Initialize the timer
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateTimer);

    // Connect button signals to slots
    connect(solveButton, &QPushButton::clicked, this, &MainWindow::onSolveClicked);
    connect(resetButton, &QPushButton::clicked, this, &MainWindow::onResetClicked);
    connect(generateButton, &QPushButton::clicked, this, &MainWindow::onGenerateClicked);
    connect(verifyButton, &QPushButton::clicked, this, &MainWindow::onVerifyClicked);

    // Set background color for the Sudoku widget using integers
    QPalette pal = sudokuWidget->palette();
    pal.setColor(QPalette::Window, QColor(224, 224, 224)); // Light gray background using integers
    sudokuWidget->setAutoFillBackground(true);
    sudokuWidget->setPalette(pal);
}



MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onStartClicked()
{
    setCentralWidget(sudokuWidget); // Switch to the Sudoku widget when START is clicked
    time.setHMS(0, 0, 0); // Reset the timer
    timerLabel->setText(time.toString("mm:ss")); // Reset the label
    timer->start(1000); // Start the timer to update every second
}

void MainWindow::updateTimer()
{
    time = time.addSecs(1); // Increment the timer by 1 second
    timerLabel->setText(time.toString("mm:ss")); // Update the label
}


void MainWindow::onSolveClicked()
{
    std::vector<std::vector<int>> board(9, std::vector<int>(9, 0));
    loadBoard(board);

    Graph graph; // Create an instance of Graph
    if (graph.solveSudoku(board)) {
        displayBoard(board);
        //QMessageBox::information(this, "Solution Found", "The Sudoku puzzle has been solved!");
    } else {
        QMessageBox::information(this, "No Solution", "The Sudoku puzzle cannot be solved.");
    }
}

void MainWindow::onResetClicked()
{
    // Stop the timer
    timer->stop();

    // Reset the time to 00:00
    time.setHMS(0, 0, 0);

    // Update the timer label
    timerLabel->setText(time.toString("mm:ss"));

    // Clear all cells in the grid and make them editable
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            cells[i][j]->clear();
            cells[i][j]->setReadOnly(false); // Make cells editable
        }
    }

    // Optionally restart the timer if needed
    timer->start(1000);
}


void MainWindow::onGenerateClicked()
{
    displayBoard(exampleBoard); // Display the example board as a generated puzzle
}

void MainWindow::onVerifyClicked()
{
    std::vector<std::vector<int>> board(9, std::vector<int>(9, 0));
    loadBoard(board);

    bool isCorrect = isSolutionCorrect(board);

    if (isCorrect) {
        QMessageBox::information(this, "Correct Solution", "Congratulations! Your solution is correct.");
    } else {
        QMessageBox::information(this, "Incorrect Solution", "Oops! There is a mistake in your solution.");
    }
}

void MainWindow::loadBoard(std::vector<std::vector<int>>& board)
{
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            QString text = cells[i][j]->text();
            if (text.isEmpty() || text == "0") {
                board[i][j] = 0;
            } else {
                board[i][j] = text.toInt();
            }
        }
    }
}

void MainWindow::displayBoard(const std::vector<std::vector<int>>& board)
{
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            if (board[i][j] != 0) {
                cells[i][j]->setText(QString::number(board[i][j]));
                cells[i][j]->setReadOnly(true); // Make cells read-only
            } else {
                cells[i][j]->clear();
                cells[i][j]->setReadOnly(false); // Make cells editable
            }
        }
    }
}

bool MainWindow::solveSudoku(std::vector<std::vector<int>>& board)
{
    int row, col;
    if (!findEmptyCell(board, row, col)) {
        return true; // Puzzle solved
    }

    for (int num = 1; num <= 9; ++num) {
        if (isValid(board, row, col, num)) {
            board[row][col] = num;
            if (solveSudoku(board)) {
                return true;
            }
            board[row][col] = 0;    //Backtrack
        }
    }
    return false; // No solution exists
}


bool MainWindow::isValid(const std::vector<std::vector<int>>& board, int row, int col, int num)
{
    // Check row and column using sets
    // std::unordered_set<int> rowSet;
    // std::unordered_set<int> colSet;
    std::vector<int> rowSet;
    std::vector<int> colSet;


    for (int x = 0; x < 9; ++x) {
        if (board[row][x] == num && x != col) {
            return false; // Duplicate number found in row
        }
        rowSet.push_back(board[row][x]);

        if (board[x][col] == num && x != row) {
            return false; // Duplicate number found in column
        }
        colSet.push_back(board[x][col]);
    }

    // Check 3x3 subgrid using a set
    int startRow = row - row % 3;
    int startCol = col - col % 3;
    std::vector<int> subgridSet;

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            int currentNum = board[i + startRow][j + startCol];
            if (currentNum == num && (i + startRow != row || j + startCol != col)) {
                return false; // Duplicate number found in subgrid
            }
            subgridSet.push_back(currentNum);
        }
    }

    return true;
}


bool MainWindow::findEmptyCell(std::vector<std::vector<int>>& board, int &row, int &col)
{
    for (row = 0; row < 9; ++row) {
        for (col = 0; col < 9; ++col) {
            if (board[row][col] == 0) {
                return true;
            }
        }
    }
    return false;
}

bool MainWindow::isSolutionCorrect(const std::vector<std::vector<int>>& board)
{
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            if (board[i][j] == 0 || !isValid(board, i, j, board[i][j])) {
                return false;
            }
        }
    }
    return true;
}

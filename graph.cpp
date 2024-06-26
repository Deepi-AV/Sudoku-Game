#include "Graph.h"
#include <QDebug>

Graph::Graph()
{
    // Initialize any required data structures or variables here
}

void Graph::addLocation(const QString &locationName)
{
    locationNames.push_back(locationName);
}

void Graph::addEdge(int src, int dest, int weight)
{
    // Implement as needed for specific use cases
    Q_UNUSED(src)
    Q_UNUSED(dest)
    Q_UNUSED(weight)
}

bool Graph::solveSudoku(std::vector<std::vector<int>>& board)
{
    int row, col;
    if (!findEmptyCell(board, row, col)) {
        return true; // All cells filled, solution found
    }
    for (int num = 1; num <= 9; ++num) {
        if (isValid(board, row, col, num)) {
            board[row][col] = num;
            if (solveSudoku(board)) {
                return true; // Recursively solve the puzzle
            }
            board[row][col] = 0; // Backtrack
        }
    }
    return false; // No valid number found for this cell, backtrack
}

bool Graph::isValid(std::vector<std::vector<int>>& board, int row, int col, int num)
{
    // Check if the number is already used in the current row, column, or 3x3 grid
    for (int x = 0; x < 9; ++x) {
        if (board[row][x] == num || board[x][col] == num) {
            return false;
        }
    }
    int startRow = row - row % 3;
    int startCol = col - col % 3;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (board[i + startRow][j + startCol] == num) {
                return false;
            }
        }
    }
    return true;
}

bool Graph::findEmptyCell(std::vector<std::vector<int>>& board, int &row, int &col)
{
    // Find the next empty cell (with 0)
    for (row = 0; row < 9; ++row) {
        for (col = 0; col < 9; ++col) {
            if (board[row][col] == 0) {
                return true;
            }
        }
    }
    return false; // No empty cell found
}

int Graph::getLocationIndex(const QString &locationName) const
{
    // Return the index of the location name in the vector
    for (int i = 0; i < locationNames.size(); ++i) {
        if (locationNames[i] == locationName) {
            return i;
        }
    }
    return -1; // Not found
}

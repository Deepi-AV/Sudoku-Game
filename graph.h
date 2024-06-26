#ifndef GRAPH_H
#define GRAPH_H

#include <QString>
#include <vector>

class Graph
{
public:
    Graph();
    void addLocation(const QString &locationName);
    void addEdge(int src, int dest, int weight);
    bool solveSudoku(std::vector<std::vector<int>>& board);
    bool isValid(std::vector<std::vector<int>>& board, int row, int col, int num);
    bool findEmptyCell(std::vector<std::vector<int>>& board, int &row, int &col);
    int getLocationIndex(const QString &locationName) const;

private:
    std::vector<QString> locationNames;
};

#endif // GRAPH_H

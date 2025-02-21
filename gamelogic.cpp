#include "gamelogic.h"
#include <QRandomGenerator>
#include <QPair>

//Places mines on the board
void GameLogic::placeMines(QVector<QVector<Cell*>>& cells, int rows, int cols, int mines)
{
    int count = 0;
    while (count < mines) {
        int r = QRandomGenerator::global()->bounded(rows);
        int c = QRandomGenerator::global()->bounded(cols);

        if (!cells[r][c]->isMine()) {
            cells[r][c]->setMine(true);
            count++;
        }
    }
}

//Calculates the number of adjacent mines to each cell
void GameLogic::calculateAdjacentMineCounts(QVector<QVector<Cell*>>& cells, int rows, int cols)
{
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            if (!cells[r][c]->isMine()) {
                int adj = countSurroundingMines(cells, rows, cols, r, c);
                cells[r][c]->setAdjacentMines(adj);
            }
        }
    }
}

//Counts the number of adjacent mines to a cell
int GameLogic::countSurroundingMines(const QVector<QVector<Cell*>>& cells, int rows, int cols, int row, int col)
{
    static const int dr[8] = { -1, -1, -1,  0,  1, 1, 1, 0 };
    static const int dc[8] = { -1,  0,  1,  1,  1, 0, -1, -1 };

    int count = 0;
    for (int i = 0; i < 8; ++i) {
        int rr = row + dr[i];
        int cc = col + dc[i];
        if (rr >= 0 && rr < rows && cc >= 0 && cc < cols) {
            if (cells[rr][cc]->isMine()) {
                count++;
            }
        }
    }
    return count;
}

//Reveals all empty cells around a cell
void GameLogic::revealEmptyArea(QVector<QVector<Cell*>>& cells, int rows, int cols, int row, int col)
{
    static const int dr[8] = { -1, -1, -1,  0,  1,  1,  1,  0 };
    static const int dc[8] = { -1,  0,  1,  1,  1,  0, -1, -1 };

    QVector<QPair<int,int>> stack;
    stack.push_back(qMakePair(row, col));

    while (!stack.isEmpty()) {
        auto [r, c] = stack.back();
        stack.pop_back();

        for (int i = 0; i < 8; ++i) {
            int rr = r + dr[i];
            int cc = c + dc[i];
            if (rr >= 0 && rr < rows && cc >= 0 && cc < cols) {
                Cell *nbr = cells[rr][cc];
                if (!nbr->isRevealed() && !nbr->isMine() && !nbr->isFlagged()) {
                    nbr->reveal();
                    if (nbr->adjacentMines() == 0) {
                        stack.push_back(qMakePair(rr, cc));
                    }
                }
            }
        }
    }
}

//Checks if the player has won the game
bool GameLogic::checkWinCondition(const QVector<QVector<Cell*>>& cells, int rows, int cols)
{
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            Cell *cell = cells[r][c];
            if (cell->isMine() && !cell->isFlagged()) {
                return false;
            }
            if (!cell->isMine() && !cell->isRevealed()) {
                return false;
            }
        }
    }
    return true;
}

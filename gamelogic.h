#ifndef GAMELOGIC_H
#define GAMELOGIC_H
#include <QVector>
#include "cell.h"

class GameLogic {
public:
    static void placeMines(QVector<QVector<Cell*>>& cells, int rows, int cols, int mines);
    static void calculateAdjacentMineCounts(QVector<QVector<Cell*>>& cells, int rows, int cols);
    static int countSurroundingMines(const QVector<QVector<Cell*>>& cells, int rows, int cols, int row, int col);
    static void revealEmptyArea(QVector<QVector<Cell*>>& cells, int rows, int cols, int row, int col);
    static bool checkWinCondition(const QVector<QVector<Cell*>>& cells, int rows, int cols);
};
#endif

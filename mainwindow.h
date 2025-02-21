#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QVector>
#include <QSpinBox>

class Cell;
class QGridLayout;
class QLabel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onCellLeftClicked(int row, int col);
    void onCellRightClicked(int row, int col);

private:
    void createBoard(int rows, int cols, int mines);
    void placeMines(int mines);
    void calculateAdjacentMineCounts();
    void resetGame();
    int  countSurroundingMines(int row, int col) const;
    void revealCell(int row, int col);
    void revealAllMines();
    void revealEmptyArea(int row, int col);
    bool checkWinCondition() const;
    void gameOver(bool won);
    void showDifficultyDialog();
    void setupDifficultyUI(QDialog* dialog);

private:
    QWidget     *m_centralWidget;
    QGridLayout *m_gridLayout;
    QLabel      *m_statusLabel;
    QSpinBox    *m_minesInput;
    int m_rows;
    int m_cols;
    static int m_totalMines;
    int m_flagsUsed;
    bool m_gameOver;
    QVector<QVector<Cell*>> m_cells;
};
#endif

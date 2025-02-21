#ifndef CELL_H
#define CELL_H
#include <QPushButton>

class Cell : public QPushButton
{
    Q_OBJECT

public:
    explicit Cell(QWidget *parent = nullptr);
    void loadIcons();
    bool isMine() const { return m_isMine; }
    void setMine(bool mine) { m_isMine = mine; }
    bool isRevealed() const { return m_isRevealed; }
    void setRevealed(bool revealed) { m_isRevealed = revealed; }
    bool isFlagged() const { return m_isFlagged; }
    void setFlagged(bool flagged) { m_isFlagged = flagged; }
    int adjacentMines() const { return m_adjacentMines; }
    void setAdjacentMines(int count) { m_adjacentMines = count; }
    int row() const { return m_row; }
    int col() const { return m_col; }
    void setPosition(int row, int col) { m_row = row; m_col = col; }
    void setDefaultIcon();
    void setMineIcon();
    void setFlagIcon();
    void reset();
    void reveal();
    void toggleFlag();

signals:
    void leftClicked(int row, int col);
    void rightClicked(int row, int col);

protected:
    void mousePressEvent(QMouseEvent *event) override;

private:
    bool m_isMine;
    bool m_isRevealed;
    bool m_isFlagged;
    int  m_adjacentMines;
    int m_row;
    int m_col;
    static QIcon m_mineIcon;
    static QIcon m_flagIcon;
    static QIcon m_defaultIcon;
    static bool m_iconsLoaded;
};
#endif

/*
 * Nathan Orgera
 * MainWindow class for Minesweeper game
 */
#include "mainwindow.h"
#include "cell.h"
#include "gamelogic.h"
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QRandomGenerator>
#include <QPushButton>
#include <QWidget>
#include <QVector>
#include <QPair>
#include <QApplication>
#include <QScreen>
#include <QGuiApplication>
#include <QSpinBox>
#include <algorithm>

int MainWindow::m_totalMines = 99;

// Constructor for main window, initializes game board and UI
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      m_centralWidget(new QWidget(this)),
      m_gridLayout(new QGridLayout),
      m_rows(16),
      m_cols(30),
      m_flagsUsed(0),
      m_gameOver(false)
{
    m_gridLayout->setSpacing(0);
    m_gridLayout->setContentsMargins(0, 0, 0, 0);
    showDifficultyDialog();
    m_centralWidget->setLayout(m_gridLayout);
    setCentralWidget(m_centralWidget);
    setWindowTitle("Minesweeper");
    int boardWidth  = m_cols * 32; 
    int boardHeight = m_rows * 32; 
    resize(boardWidth, boardHeight);
    setFixedSize(boardWidth, boardHeight); 
}

// Destructor for main window, cleans up cells
MainWindow::~MainWindow()
{
    for (int r = 0; r < m_rows; ++r) {
        for (int c = 0; c < m_cols; ++c) {
            delete m_cells[r][c];
        }
    }
}

// Helper to fully reset the board
void MainWindow::resetGame()
{
    QLayoutItem *item;
    while ((item = m_gridLayout->takeAt(0)) != nullptr) {
        if (QWidget *widget = item->widget()) {
            widget->deleteLater();
        }
        delete item;
    }
    for (int r = 0; r < m_rows; ++r) {
        for (int c = 0; c < m_cols; ++c) {
            if (m_cells[r][c]) {
                m_cells[r][c]->deleteLater();
            }
        }
    }
    m_cells.clear();
    m_flagsUsed = 0;
    m_gameOver = false;
    createBoard(m_rows, m_cols, m_totalMines);
}

// Creates a new game board with specified dimensions and mines
void MainWindow::createBoard(int rows, int cols, int mines)
{
    m_rows       = rows;
    m_cols       = cols;
    m_totalMines = mines;
    m_flagsUsed  = 0;
    m_gameOver   = false;

    // Clear old layout items if any
    QLayoutItem *item;
    while ((item = m_gridLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    // Initialize m_cells
    m_cells.resize(rows);
    for (int r = 0; r < rows; ++r) {
        m_cells[r].resize(cols);
    }

    // Create and place Cell widgets
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            Cell *cell = new Cell(this);
            cell->setPosition(r, c);
            cell->setText(""); // for numbers
            cell->setChecked(false);
            cell->setFlagged(false);
            cell->setRevealed(false);
            cell->setMine(false);
            cell->setDefaultIcon();
            connect(cell, &Cell::leftClicked, this, &MainWindow::onCellLeftClicked);
            connect(cell, &Cell::rightClicked, this, &MainWindow::onCellRightClicked);
            m_cells[r][c] = cell;
            m_gridLayout->addWidget(cell, r, c);
        }
    }
    placeMines(mines);
    calculateAdjacentMineCounts();
}

// Places mines on the board
void MainWindow::placeMines(int mines)
{
    GameLogic::placeMines(m_cells, m_rows, m_cols, mines);
}

// Calculates the number of adjacent mines for each cell
void MainWindow::calculateAdjacentMineCounts()
{
    GameLogic::calculateAdjacentMineCounts(m_cells, m_rows, m_cols);
}

// Counts the number of surrounding mines for a cell
int MainWindow::countSurroundingMines(int row, int col) const
{
    return GameLogic::countSurroundingMines(m_cells, m_rows, m_cols, row, col);
}

// Handles left click events on a cell
void MainWindow::onCellLeftClicked(int row, int col)
{
    if (m_gameOver) return;
    Cell *cell = m_cells[row][col];
    if (cell->isFlagged() || cell->isRevealed()) return;
    if (cell->isMine()) {
        revealAllMines();
        gameOver(false);
        return;
    }
    revealCell(row, col);
    if (cell->adjacentMines() == 0) {
        revealEmptyArea(row, col);
    }
    if (checkWinCondition()) {
        gameOver(true);
    }
}

// Handles right click events on a cell
void MainWindow::onCellRightClicked(int row, int col)
{
    if (m_gameOver) return;
    Cell *cell = m_cells[row][col];
    if (cell->isRevealed()) return;
    if (cell->isFlagged()) {
        cell->setFlagged(false);
        cell->setDefaultIcon();
        m_flagsUsed--;
    } else {
        cell->setFlagged(true);
        cell->setFlagIcon();
        m_flagsUsed++;
    }
    if (checkWinCondition()) {
        gameOver(true);
    }
}

// Reveals a cell and updates the UI
void MainWindow::revealCell(int row, int col)
{
    Cell *cell = m_cells[row][col];
    if (cell->isRevealed()) return;
    cell->reveal();
}

// Reveals all mines on the board
void MainWindow::revealAllMines()
{
    for (int r = 0; r < m_rows; ++r) {
        for (int c = 0; c < m_cols; ++c) {
            Cell *cell = m_cells[r][c];
            if (cell->isMine()) {
                cell->setMineIcon();
            }
        }
    }
}

// Reveals all empty cells around a cell
void MainWindow::revealEmptyArea(int row, int col)
{
    GameLogic::revealEmptyArea(m_cells, m_rows, m_cols, row, col);
}

// Checks if the player has won the game
bool MainWindow::checkWinCondition() const
{
    return GameLogic::checkWinCondition(m_cells, m_rows, m_cols);
}

// Handles the game over logic
void MainWindow::gameOver(bool won)
{
    m_gameOver = true;
    QMessageBox msgBox;
    if (won) {
        msgBox.setText("You cleared all mines!");
    } else {
        msgBox.setText("You hit a mine!");
    }
    QPushButton *againBtn = msgBox.addButton(tr("Play Again"), QMessageBox::ActionRole);
    QPushButton *menuBtn = msgBox.addButton(tr("Main Menu"), QMessageBox::ActionRole);
    QPushButton *exitBtn = msgBox.addButton(tr("Exit"), QMessageBox::ActionRole);
    msgBox.exec();
    if (msgBox.clickedButton() == againBtn) {
        resetGame();
    } else if (msgBox.clickedButton() == menuBtn) {
        int currentMines = m_totalMines;
        close();
        MainWindow *newWindow = new MainWindow();
        newWindow->show();
    } else if (msgBox.clickedButton() == exitBtn) {
        QApplication::quit();
    }
}

// Shows the difficulty dialog
void MainWindow::showDifficultyDialog()
{
    QDialog dialog(this);
    dialog.setWindowTitle("Minesweeper");
    dialog.setModal(true);
    setupDifficultyUI(&dialog);
    dialog.adjustSize();
    
    // Center the dialog on screen
    QScreen *screen = QGuiApplication::primaryScreen();
    if (screen) {
        QRect screenGeometry = screen->geometry();
        int x = (screenGeometry.width() - dialog.width()) / 2;
        int y = (screenGeometry.height() - dialog.height()) / 2;
        dialog.move(x, y);
    }
    if (dialog.exec() != QDialog::Accepted) {
        deleteLater();
        QMetaObject::invokeMethod(qApp, "quit", Qt::QueuedConnection);
        return;
    }
}

// Sets up the difficulty UI
void MainWindow::setupDifficultyUI(QDialog* dialog)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(dialog);
    QLabel *welcomeLabel = new QLabel("Welcome to Minesweeper!", dialog);
    welcomeLabel->setAlignment(Qt::AlignCenter);
    QFont font = welcomeLabel->font();
    font.setPointSize(14);
    font.setBold(true);
    welcomeLabel->setFont(font);
    QPushButton *standardBtn = new QPushButton("Normal Mode", dialog);
    connect(standardBtn, &QPushButton::clicked, [this, dialog]() {
        m_totalMines = 99;
        dialog->accept();
        createBoard(16, 30, m_totalMines);
    });
    
    QHBoxLayout *customLayout = new QHBoxLayout;
    QPushButton *customBtn = new QPushButton("Custom Bombs", dialog);
    customBtn->setStyleSheet(standardBtn->styleSheet());
    m_minesInput = new QSpinBox(dialog);
    m_minesInput->setRange(INT_MIN, INT_MAX);
    m_minesInput->setValue(m_totalMines);
    connect(customBtn, &QPushButton::clicked, [this, dialog]() { //Custom bombs button & range logic
        int mines = m_minesInput->value();
        if (mines < 1) {
            mines = 1;
        } else if (mines > 479) {
            mines = 479;
        }
        m_totalMines = mines;
        dialog->accept();
        createBoard(16, 30, m_totalMines);
    });
    customLayout->addWidget(customBtn);
    customLayout->addWidget(m_minesInput);
    mainLayout->addWidget(welcomeLabel);
    mainLayout->addSpacing(20);
    mainLayout->addWidget(standardBtn);
    mainLayout->addSpacing(10);
    mainLayout->addLayout(customLayout);
    dialog->setFixedSize(300, 200);
}

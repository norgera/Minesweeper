#include "cell.h"
#include <QMouseEvent>
#include <QFile>

QIcon Cell::m_mineIcon;
QIcon Cell::m_flagIcon;
QIcon Cell::m_defaultIcon;
bool Cell::m_iconsLoaded = false;

//Cell constructor
Cell::Cell(QWidget *parent)
    : QPushButton(parent),
      m_isMine(false),
      m_isRevealed(false),
      m_isFlagged(false),
      m_adjacentMines(0),
      m_row(-1),
      m_col(-1)
{
    setFixedSize(32, 32);
    setCheckable(true);
    
    setStyleSheet(
        "QPushButton {"
        "    background-color: #c0c0c0;"
        "    border: 1px outset #808080;"
        "}"
        "QPushButton:checked {"
        "    background-color: #d8d8d8;"
        "    border: 0px;"
        "}"
    );
    
    if (!m_iconsLoaded) {
        loadIcons();
    }
    setIconSize(QSize(32, 32));
}


//Loads icons
void Cell::loadIcons()
{
    m_mineIcon = QIcon(":/icons/mine.png");
    m_flagIcon = QIcon(":/icons/flag.png");
    m_defaultIcon = QIcon(":/icons/default.png");
    m_iconsLoaded = true;
}

// Differentiate left/right click
void Cell::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        emit leftClicked(m_row, m_col);
    }
    else if (event->button() == Qt::RightButton) {
        emit rightClicked(m_row, m_col);
    }
    QPushButton::mousePressEvent(event);
}

void Cell::setDefaultIcon()
{
    setIcon(m_defaultIcon);
}

void Cell::setMineIcon()
{
    setIcon(m_mineIcon);
}

void Cell::setFlagIcon()
{
    setIcon(m_flagIcon);
}

//Resets cell to default state
void Cell::reset()
{
    setText("");
    setChecked(false);
    setFlagged(false);
    setRevealed(false);
    setMine(false);
    setDefaultIcon();
}

//Reveals cell
void Cell::reveal()
{
    if (isRevealed()) return;
    setRevealed(true);
    setChecked(true);
    setIcon(QIcon());
    if (adjacentMines() > 0) { //If cell has adjacent mines, set text to number of adjacent mines
        setText(QString::number(adjacentMines()));
        QString colors[] = {"", "#0000ff", "#008000", "#ff0000", "#000080", 
                          "#800000", "#008080", "#000000", "#808080"};
        setStyleSheet(QString(
            "QPushButton {"
            "    color: %1;"
            "    background-color: #d8d8d8;"
            "    border: 1px solid #808080;"
            "    font-weight: bold;"
            "}"
        ).arg(colors[adjacentMines()]));
    } else { //If cell has no adjacent mines, set text to empty string
        setText("");
        setStyleSheet(
            "QPushButton {"
            "    background-color: #d8d8d8;"
            "    border: 0px;"
            "}"
        );
    }
}

//Toggles flag
void Cell::toggleFlag()
{
    if (isRevealed()) return;

    setFlagged(!isFlagged());
    if (isFlagged()) {
        setFlagIcon();
    } else {
        setDefaultIcon();
    }
}

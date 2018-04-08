#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    createIcons();
    createActions();
    createToolBar();
    createBoard();

    setCentralWidget(m_Board);
    setMinimumSize(500,500);
    setWindowTitle("AutoCell");
}

void MainWindow::createIcons(){
    QPixmap fastBackwardPm(":/icons/icons/fast-backward.svg");
    QPixmap fastBackwardHoveredPm(":/icons/icons/fast-backward-full.svg");
    QPixmap fastForwardPm(":/icons/icons/fast-forward.svg");
    QPixmap fastForwardHoveredPm(":/icons/icons/fast-forward-full.svg");
    QPixmap playPm(":/icons/icons/play.svg");
    QPixmap playHoveredPm(":/icons/icons/play-full.svg");
    QPixmap newPm(":/icons/icons/new.svg");
    QPixmap openPm(":/icons/icons/open.svg");
    QPixmap savePm(":/icons/icons/save.svg");
    QPixmap pausePm(":/icons/icons/pause.svg");

    m_fastBackwardIcon.addPixmap(fastBackwardPm, QIcon::Normal, QIcon::Off);
    m_fastBackwardIcon.addPixmap(fastBackwardHoveredPm, QIcon::Active, QIcon::Off);
    m_fastForwardIcon.addPixmap(fastForwardPm, QIcon::Normal, QIcon::Off);
    m_fastForwardIcon.addPixmap(fastForwardHoveredPm, QIcon::Active, QIcon::Off);
    m_playIcon.addPixmap(playPm, QIcon::Normal, QIcon::Off);
    m_playIcon.addPixmap(playHoveredPm, QIcon::Active, QIcon::Off);
    m_pauseIcon.addPixmap(pausePm, QIcon::Normal, QIcon::Off);
    m_newIcon.addPixmap(newPm, QIcon::Normal, QIcon::Off);
    m_saveIcon.addPixmap(savePm, QIcon::Normal, QIcon::Off);
    m_openIcon.addPixmap(openPm, QIcon::Normal, QIcon::Off);
}

void MainWindow::createActions(){
    m_fastBackward = new QAction(m_fastBackwardIcon, tr("&fast backward"), this);
    m_fastForward = new QAction(m_fastForwardIcon, tr("&fast forward"), this);
    m_playPause = new QAction(m_playIcon, tr("Play"), this);
    m_saveAutomaton = new QAction(m_saveIcon, tr("Save automaton"), this);
    m_newAutomaton = new QAction(m_newIcon, tr("New automaton"), this);
    m_openAutomaton = new QAction(m_openIcon, tr("Open automaton"), this);
}

void MainWindow::createToolBar(){
    QToolBar *m_toolBar = addToolBar("toolbar");
    QLabel *m_speedLabel = new QLabel(tr("  Speed :  "));
    QLineEdit *m_jumpSpeed = new QLineEdit();
    m_jumpSpeed->setFixedWidth(40);
    m_toolBar->setMovable(false);

    m_toolBar->addWidget(new QLabel("   "));
    m_toolBar->addAction(m_newAutomaton);
    m_toolBar->addAction(m_openAutomaton);
    m_toolBar->addAction(m_saveAutomaton);
    m_toolBar->addWidget(new QLabel("   "));
    m_toolBar->addSeparator();
    m_toolBar->addAction(m_fastBackward);
    m_toolBar->addAction(m_playPause);
    m_toolBar->addAction(m_fastForward);
    m_toolBar->addSeparator();
    m_toolBar->addWidget(m_speedLabel);
    m_toolBar->addWidget(m_jumpSpeed);


}

void MainWindow::createBoard(){
    m_Board = new QTableWidget(m_boardSize, m_boardSize, this);
        m_Board->setFixedSize(m_boardSize*m_cellSize, m_boardSize*m_cellSize);
        m_Board->horizontalHeader()->setVisible(false);
        m_Board->verticalHeader()->setVisible(false);
        m_Board->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        m_Board->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        m_Board->setEditTriggers(QAbstractItemView::NoEditTriggers); // désactive la modification par l'utilisateur
        // on va créer les items, on utilise 2 boucles car on parcourt un tableau 2 m_boardSizes
        for(unsigned int row = 0; row < m_boardSize; ++row) {
            // fixe les m_boardSizes des rows et des cols
            m_Board->setColumnWidth(row, m_cellSize);
            m_Board->setRowHeight(row, m_cellSize);
            for(unsigned int col = 0; col < m_boardSize; ++col) {
                m_Board->setItem(row, col, new QTableWidgetItem(""));
                m_Board->item(row, col)->setBackgroundColor("white");
                m_Board->item(row, col)->setTextColor("white");
            }
        }
}

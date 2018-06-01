#include "mainwindow.h"
#include <iostream>
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    createIcons();
    createActions();
    createToolBar();


    setMinimumSize(500,500);
    setWindowTitle("AutoCell");

    m_tabs = NULL;
}

/** \fn MainWindow::createIcons()
 * \brief Creates Icons for the MainWindow
 */


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
    QPixmap resetPm(":/icons/icons/reset.svg");

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
    m_resetIcon.addPixmap(resetPm, QIcon::Normal, QIcon::Off);
}

/** \fn MainWindow::createActions()
 * \brief Creates and connects QActions and associated buttons for the MainWindow
 */

void MainWindow::createActions(){
    m_fastBackward = new QAction(m_fastBackwardIcon, tr("&fast backward"), this);
    m_fastForward = new QAction(m_fastForwardIcon, tr("&fast forward"), this);
    m_playPause = new QAction(m_playIcon, tr("Play"), this);
    m_saveAutomaton = new QAction(m_saveIcon, tr("Save automaton"), this);
    m_newAutomaton = new QAction(m_newIcon, tr("New automaton"), this);
    m_openAutomaton = new QAction(m_openIcon, tr("Open automaton"), this);
    m_resetAutomaton = new QAction(m_resetIcon, tr("Reset automaton"), this);



    m_fastBackwardBt = new QToolButton(this);
    m_fastForwardBt = new QToolButton(this);
    m_playPauseBt = new QToolButton(this);
    m_saveAutomatonBt = new QToolButton(this);
    m_newAutomatonBt = new QToolButton(this);
    m_openAutomatonBt = new QToolButton(this);
    m_resetBt = new QToolButton(this);

    m_fastBackwardBt->setDefaultAction(m_fastBackward);
    m_fastForwardBt->setDefaultAction(m_fastForward);
    m_playPauseBt->setDefaultAction(m_playPause);
    m_saveAutomatonBt->setDefaultAction(m_saveAutomaton);
    m_newAutomatonBt->setDefaultAction(m_newAutomaton);
    m_openAutomatonBt->setDefaultAction(m_openAutomaton);
    m_resetBt->setDefaultAction(m_resetAutomaton);

    m_fastBackwardBt->setIconSize(QSize(30,30));
    m_fastForwardBt->setIconSize(QSize(30,30));
    m_playPauseBt->setIconSize(QSize(30,30));
    m_saveAutomatonBt->setIconSize(QSize(30,30));
    m_newAutomatonBt->setIconSize(QSize(30,30));
    m_openAutomatonBt->setIconSize(QSize(30,30));
    m_resetBt->setIconSize(QSize(30,30));

    connect(m_openAutomatonBt, SIGNAL(clicked(bool)), this, SLOT(openFile()));
    connect(m_newAutomatonBt, SIGNAL(clicked(bool)), this, SLOT(openCreationWindow()));
    connect(m_saveAutomatonBt, SIGNAL(clicked(bool)), this, SLOT(saveToFile()));
    connect(m_fastForwardBt, SIGNAL(clicked(bool)), this, SLOT(forward()));

}

/** \fn MainWindow::createToolBar()
 * \brief Creates the toolBar for the MainWindow
 */

void MainWindow::createToolBar(){
    m_toolBar = new QToolBar(this);
    QLabel *m_speedLabel = new QLabel(tr("Speed : "),this);
    m_jumpSpeed = new QSpinBox(this);
    m_jumpSpeed->setValue(1);
    m_speedLabel->setFixedWidth(50);
    m_jumpSpeed->setFixedWidth(40);
    m_toolBar->setMovable(false);

    QHBoxLayout *tbLayout = new QHBoxLayout(this);
    tbLayout->addWidget(m_newAutomatonBt, Qt::AlignCenter);
    tbLayout->addWidget(m_openAutomatonBt, Qt::AlignCenter);
    tbLayout->addWidget(m_saveAutomatonBt, Qt::AlignCenter);
    tbLayout->addWidget(m_fastBackwardBt, Qt::AlignCenter);
    tbLayout->addWidget(m_playPauseBt, Qt::AlignCenter);
    tbLayout->addWidget(m_fastForwardBt, Qt::AlignCenter);
    tbLayout->addWidget(m_speedLabel, Qt::AlignCenter);
    tbLayout->addWidget(m_jumpSpeed, Qt::AlignCenter);
    tbLayout->addWidget(m_resetBt, Qt::AlignCenter);


    tbLayout->setAlignment(Qt::AlignCenter);
    QWidget* wrapper = new QWidget(this);
    wrapper->setLayout(tbLayout);
    m_toolBar->addWidget(wrapper);
    addToolBar(m_toolBar);


}

/** \fn MainWindow::createTab()
 * \brief Creates a new Tab with an empty board
 */

QWidget* MainWindow::createTab(){
    QWidget *tab = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(this);

    QVector<unsigned int> dimensions = m_cellHandlers.last()->getDimensions();
    int boardVSize = 0;
    int boardHSize = 0;
    if(dimensions.size() > 1){
        boardVSize = dimensions[0];
        boardHSize = dimensions[1];
    }
    else{
        boardVSize = 1;
        boardHSize = dimensions[0];
    }

    QTableWidget* board = new QTableWidget(boardVSize, boardHSize, this);
        board->setFixedSize(boardHSize*m_cellSize,boardVSize*m_cellSize);
        //setMinimumSize(m_boardHSize*m_cellSize,100+m_boardVSize*m_cellSize);
        board->horizontalHeader()->setVisible(false);
        board->verticalHeader()->setVisible(false);
        board->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        board->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        board->setEditTriggers(QAbstractItemView::NoEditTriggers);
        for(unsigned int col = 0; col < boardHSize; ++col)
            board->setColumnWidth(col, m_cellSize);
        for(unsigned int row = 0; row < boardVSize; ++row) {
            board->setRowHeight(row, m_cellSize);
            for(unsigned int col = 0; col < boardHSize; ++col) {
                board->setItem(row, col, new QTableWidgetItem(""));
                board->item(row, col)->setBackgroundColor("white");
                board->item(row, col)->setTextColor("black");
            }
        }
     QScrollArea *scrollArea = new QScrollArea(this);
     scrollArea->setWidget(board);
     layout->setContentsMargins(0,0,0,0);
     layout->addWidget(scrollArea);
     tab->setLayout(layout);
     return tab;
}

/** \fn MainWindow::openFile()
 * \brief Opens a file browser for the user to select automaton files and creates an automaton
 */
void MainWindow::openFile(){
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Cell file"), ".",
                                                    tr("Automaton cell files (*.atc)"));
    if(!fileName.isEmpty()){
        m_cellHandlers.append(new CellHandler(fileName));
        std::cout << "m_cellHandlers size :" <<m_cellHandlers.size() << std::endl<<std::flush;
        if(m_tabs == NULL) createTabs();
        m_tabs->addTab(createTab(), "Automaton "+ QString::number(m_cellHandlers.size()));
        updateBoard(m_cellHandlers.size()-1);
    }
}


/** \fn MainWindow::saveToFile()
 * \brief Allows user to select a location and saves automaton's state and settings
 */
void MainWindow::saveToFile(){
    if(m_cellHandlers.size() > 0){
        QString fileName = QFileDialog::getSaveFileName(this, tr("Save Automaton"),
                                                        ".", tr("Automaton Cells file (*.atc"));
        m_cellHandlers[m_tabs->currentIndex()]->save(fileName+".atc");

    }
    else{
        QMessageBox msgBox;
        msgBox.critical(0,"Error","Please create or import an Automaton first !");
        msgBox.setFixedSize(500,200);
    }
}

/** \fn MainWindow::openCreationWindow()
 * \brief Opens the automaton creation window
 */

void MainWindow::openCreationWindow(){
    CreationDialog *window = new CreationDialog(this);
    connect(window, SIGNAL(settingsFilled(QVector<uint>,CellHandler::generationTypes,uint,uint)),
            this, SLOT(setCellHandler(QVector<uint>,CellHandler::generationTypes,uint,uint)));
    window->show();
}

/** \fn MainWindow::setCellHandler(const QVector<unsigned int> dimensions,
                                CellHandler::generationTypes type,
                                unsigned int stateMax, unsigned int density)
 * \brief Creates a new cellHandler with the provided arguments and updates the board with the created cellHandler
 */

void MainWindow::setCellHandler(const QVector<unsigned int> dimensions,
                                CellHandler::generationTypes type,
                                unsigned int stateMax, unsigned int density){
    CellHandler* newCellHandler = new CellHandler(dimensions, type, stateMax, density);

    if(m_tabs == NULL) createTabs();

    m_cellHandlers.append(newCellHandler);
    std::cout << "m_cellHandlers size :" <<m_cellHandlers.size() << std::endl<<std::flush;
    QWidget* newTab = createTab();
    m_tabs->addTab(newTab, "Automaton "+ QString::number(m_cellHandlers.size()));
    m_tabs->setCurrentWidget(newTab);
    updateBoard(m_cellHandlers.size()-1);

}

/** \fn MainWindow::nextState(int n)
 * \brief Shows the nth next state of the automaton on the board
 */

void MainWindow::nextState(int n){
    if(m_cellHandlers.size() == 0){
        QMessageBox msgBox;
        msgBox.critical(0,"Error","Please create or import an Automaton first !");
        msgBox.setFixedSize(500,200);
    }
    else{
        for(unsigned int i = 0; i < n; i++) m_cellHandlers[m_tabs->currentIndex()]->nextStates();
        updateBoard(m_tabs->currentIndex());
    }
}

/** \fn MainWindow::updateBoard()
 * \brief Updates cells on the board on the tab at the give index with the cellHandler's cells states
 */

void MainWindow::updateBoard(int index){
    if(m_cellHandlers.size()==0){
        QMessageBox msgBox;
        msgBox.critical(0,"Error","Please create or import an Automaton first !");
        msgBox.setFixedSize(500,200);
    }
    else{

        CellHandler* cellHandler = m_cellHandlers[index];
        QVector<unsigned int> dimensions = cellHandler->getDimensions();
        QTableWidget* board = getBoard(index);
        if(dimensions.size() > 1){
            int i = 0;
            int j = 0;
            for (CellHandler::const_iterator it = CellHandler::const_iterator(cellHandler); it != cellHandler->end() && it.changedDimension() < 2; ++it){
                    if(it.changedDimension() > 0){
                        i = 0;
                        j++;
                        std::cout << std::endl;
                    }
                    board->item(i,j)->setText(QString::number(it->getState()));
                    i++;
            }
        }
        else{
            int i = 0;
            int j = 0;
            for (CellHandler::const_iterator it = CellHandler::const_iterator(cellHandler); it != cellHandler->end() && it.changedDimension() < 1; ++it){
                    board->item(i,j)->setText(QString::number(it->getState()));
                    j++;
            }
        }

    }

}

/** \fn MainWindow::forward()
 * \brief Skips the number of steps chosen by the user and sets the automaton on the last one
 */

void MainWindow::forward(){
    nextState(m_jumpSpeed->value());
}

QTableWidget* MainWindow::getBoard(int n){
    return m_tabs->widget(n)->findChild<QTableWidget *>();
}

/** \fn MainWindow::createTabs()
 * \brief Creates a QTabWidget for the main window and displays it
 */

void MainWindow::createTabs(){
    m_tabs = new QTabWidget(this);
    m_tabs->setMovable(true);
    m_tabs->setTabsClosable(true);
    setCentralWidget(m_tabs);
    connect(m_tabs, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab(int)));
}

/** \fn MainWindow::closeTab(int n)
 * \brief Closes the tab at index n. Before closing, prompts the user to save the automaton
 */

void MainWindow::closeTab(int n){
    m_tabs->setCurrentIndex(n);
    saveToFile();
    m_tabs->removeTab(n);
}

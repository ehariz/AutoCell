#include "mainwindow.h"
#include <iostream>
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    createIcons();
    createActions();
    createToolBar();
    createBoard();


    setMinimumSize(500,500);
    setWindowTitle("AutoCell");

    m_cellHandler = NULL;
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



    m_fastBackwardBt = new QToolButton();
    m_fastForwardBt = new QToolButton();
    m_playPauseBt = new QToolButton();
    m_saveAutomatonBt = new QToolButton();
    m_newAutomatonBt = new QToolButton();
    m_openAutomatonBt = new QToolButton();
    m_resetBt = new QToolButton();

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
    QLabel *m_speedLabel = new QLabel(tr("Speed : "));
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
    QWidget* wrapper = new QWidget();
    wrapper->setLayout(tbLayout);
    m_toolBar->addWidget(wrapper);
    addToolBar(m_toolBar);


}

/** \fn MainWindow::createBoard()
 * \brief Creates the Automaton board
 */

void MainWindow::createBoard(){
    m_Board = new QTableWidget(m_boardVSize, m_boardHSize, this);
        m_Board->setFixedSize(m_boardHSize*m_cellSize,m_boardVSize*m_cellSize);
        //setMinimumSize(m_boardHSize*m_cellSize,100+m_boardVSize*m_cellSize);
        m_Board->horizontalHeader()->setVisible(false);
        m_Board->verticalHeader()->setVisible(false);
        m_Board->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        m_Board->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        m_Board->setEditTriggers(QAbstractItemView::NoEditTriggers);
        for(unsigned int col = 0; col < m_boardHSize; ++col)
            m_Board->setColumnWidth(col, m_cellSize);
        for(unsigned int row = 0; row < m_boardVSize; ++row) {
            m_Board->setRowHeight(row, m_cellSize);
            for(unsigned int col = 0; col < m_boardHSize; ++col) {
                m_Board->setItem(row, col, new QTableWidgetItem(""));
                m_Board->item(row, col)->setBackgroundColor("white");
                m_Board->item(row, col)->setTextColor("black");
            }
        }
     QScrollArea *scrollArea = new QScrollArea(this);
     scrollArea->setWidget(m_Board);
     setCentralWidget(scrollArea);
}


/** \fn MainWindow::openFile()
 * \brief Opens a file browser for the user to select automaton files and creates an automaton
 */
void MainWindow::openFile(){
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Cell file"), ".",
                                                    tr("Automaton cell files (*.atc)"));
    if(!fileName.isEmpty()){
        m_cellHandler = new CellHandler(fileName);
        QVector<unsigned int> dimensions = m_cellHandler->getDimensions();
        if(dimensions.size() > 1){
            m_boardVSize = dimensions[0];
            m_boardHSize = dimensions[1];
        }
        else{
            m_boardVSize = 1;
            m_boardHSize = dimensions[0];
        }
        createBoard();
        updateBoard();
    }
}


/** \fn MainWindow::saveToFile()
 * \brief Allows user to select a location and saves automaton's state and settings
 */
void MainWindow::saveToFile(){
    if(m_cellHandler != NULL){
        QString fileName = QFileDialog::getSaveFileName(this, tr("Save Automaton"),
                                                        ".", tr("Automaton Cells file (*.atc"));
        m_cellHandler->save(fileName+".atc");

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
    m_cellHandler = new CellHandler(dimensions, type, stateMax, density);
    if(dimensions.size() > 1){
        m_boardVSize = dimensions[0];
        m_boardHSize = dimensions[1];
    }
    else{
        m_boardVSize = 1;
        m_boardHSize = dimensions[0];
    }
    createBoard();
    updateBoard();
}

/** \fn MainWindow::nextState(int n)
 * \brief Shows the nth next state of the automaton on the board
 */

void MainWindow::nextState(int n){
    if(m_cellHandler == NULL){
        QMessageBox msgBox;
        msgBox.critical(0,"Error","Please create or import an Automaton first !");
        msgBox.setFixedSize(500,200);
    }
    else{
        for(unsigned int i = 0; i < n; i++) m_cellHandler->nextStates();
        updateBoard();
    }
}

/** \fn MainWindow::updateBoard()
 * \brief Updates cells on the board with the cellHandler's cells states
 */

void MainWindow::updateBoard(){
    if(m_cellHandler == NULL){
        QMessageBox msgBox;
        msgBox.critical(0,"Error","Please create or import an Automaton first !");
        msgBox.setFixedSize(500,200);
    }
    else{
        int i = 0;
        int j = 0;
        for (CellHandler::iterator it = m_cellHandler->begin(); it != m_cellHandler->end() && it.changedDimension() < 2; ++it){
                if(it.changedDimension() > 0){
                    i = 0;
                    j++;
                    std::cout << std::endl;
                }
                m_Board->item(i,j)->setText(QString::number(it->getState()));
                i++;
        }
    }

}

/** \fn MainWindow::forward()
 * \brief Skips the number of steps chosen by the user and sets the automaton on the last one
 */

void MainWindow::forward(){
    nextState(m_jumpSpeed->value());
}

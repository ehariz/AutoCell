#include "mainwindow.h"
#include <iostream>
#include "math.h"
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    createIcons();
    createActions();
    createToolBar();


    setMinimumSize(500,500);
    setWindowTitle("AutoCell");

    m_tabs = NULL;
    running = false;
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
    m_fastBackward = new QAction(m_fastBackwardIcon, tr("&Previous state"), this);
    m_fastForward = new QAction(m_fastForwardIcon, tr("&Next state"), this);
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


    m_zoom = new QSlider(Qt::Horizontal);
    m_zoom->setValue(m_cellSize);
    m_zoom->setMinimum(4);
    m_zoom->setMaximum(100);
    m_zoom->setFixedWidth(100);


    connect(m_openAutomatonBt, SIGNAL(clicked(bool)), this, SLOT(openFile()));
    connect(m_newAutomatonBt, SIGNAL(clicked(bool)), this, SLOT(openCreationWindow()));
    connect(m_saveAutomatonBt, SIGNAL(clicked(bool)), this, SLOT(saveToFile()));
    connect(m_fastForwardBt, SIGNAL(clicked(bool)), this, SLOT(forward()));
    connect(m_fastBackwardBt, SIGNAL(clicked(bool)), this, SLOT(backward()));
    connect(m_playPauseBt, SIGNAL(clicked(bool)), this, SLOT(handlePlayPause()));
    connect(m_resetBt,SIGNAL(clicked(bool)), this,SLOT(reset()));
    connect(m_zoom, SIGNAL(valueChanged(int)), this, SLOT(setSize(int)));

}

/** \fn MainWindow::createToolBar()
 * \brief Creates the toolBar for the MainWindow
 */

void MainWindow::createToolBar(){
    m_toolBar = new QToolBar(this);
    QLabel *timeStepLabel = new QLabel(tr("Timestep(ms)"),this);
    m_timeStep = new QSpinBox(this);
    m_timeStep->setMaximum(10000);
    m_timeStep->setValue(500);
    timeStepLabel->setFixedWidth(90);
    m_timeStep->setFixedWidth(60);
    m_toolBar->setMovable(false);

    QLabel *cellSetterLabel = new QLabel(tr("Cell value"));
    m_cellSetter = new QSpinBox(this);
    connect(m_cellSetter, SIGNAL(valueChanged(int)),this, SLOT(changeCellValue()));
    QLabel *zoomLabel = new QLabel(tr("Zoom"),this);
    QVBoxLayout* zoomLayout = new QVBoxLayout();
    zoomLayout->addWidget(zoomLabel, Qt::AlignCenter);
    zoomLayout->addWidget(m_zoom, Qt::AlignCenter);

    QVBoxLayout* tsLayout = new QVBoxLayout();
    tsLayout->addWidget(timeStepLabel, Qt::AlignCenter);
    tsLayout->addWidget(m_timeStep, Qt::AlignCenter);

    QVBoxLayout* csLayout = new QVBoxLayout();
    csLayout->addWidget(cellSetterLabel, Qt::AlignCenter);
    csLayout->addWidget(m_cellSetter, Qt::AlignCenter);

    QHBoxLayout *tbLayout = new QHBoxLayout(this);
    tbLayout->addLayout(zoomLayout);
    tbLayout->addWidget(m_newAutomatonBt, Qt::AlignCenter);
    tbLayout->addWidget(m_openAutomatonBt, Qt::AlignCenter);
    tbLayout->addWidget(m_saveAutomatonBt, Qt::AlignCenter);
    tbLayout->addWidget(m_fastBackwardBt, Qt::AlignCenter);
    tbLayout->addWidget(m_playPauseBt, Qt::AlignCenter);
    tbLayout->addWidget(m_fastForwardBt, Qt::AlignCenter);
    tbLayout->addWidget(m_resetBt, Qt::AlignCenter);
    tbLayout->addLayout(tsLayout);
    tbLayout->addLayout(csLayout);



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
    QVector<unsigned int> dimensions = AutomateHandler::getAutomateHandler().getAutomate(AutomateHandler::getAutomateHandler().getNumberAutomates()-1)->getCellHandler().getDimensions();
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
     connect(board, SIGNAL(cellClicked(int,int)), this, SLOT(cellPressed(int,int)));
     return tab;
}

/** \fn MainWindow::openFile()
 * \brief Opens a file browser for the user to select automaton files and creates an automaton
 */
void MainWindow::openFile(){
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Cell file"), ".",
                                                    tr("Automaton cell files (*.atc)"));
    if(!fileName.isEmpty()){
        AutomateHandler::getAutomateHandler().addAutomate(new Automate(fileName));
        if(m_tabs == NULL) createTabs();
        m_tabs->addTab(createTab(), "Automaton "+ QString::number(AutomateHandler::getAutomateHandler().getNumberAutomates()+1));
        updateBoard(AutomateHandler::getAutomateHandler().getNumberAutomates()-1);

        RuleEditor* ruleEditor = new RuleEditor();
        connect(ruleEditor, SIGNAL(fileImported(QString)),this,SLOT(addAutomatonRuleFile(QString)));
        connect(ruleEditor, SIGNAL(rulesFilled(QList<const NeighbourRule*>)), this, SLOT(addAutomatonRules(QList<const NeighbourRule*>)));
        ruleEditor->show();
    }
}


/** \fn MainWindow::saveToFile()
 * \brief Allows user to select a location and saves automaton's state and settings
 */
void MainWindow::saveToFile(){
    if(AutomateHandler::getAutomateHandler().getNumberAutomates() > 0){
        QString automatonFileName = QFileDialog::getSaveFileName(this, tr("Save Automaton cell configuration"),
                                                        ".", tr("Automaton Cells file (*.atc"));
        AutomateHandler::getAutomateHandler().getAutomate(m_tabs->currentIndex())->saveCells(automatonFileName+".atc");
        QString ruleFileName = QFileDialog::getSaveFileName(this, tr("Save Automaton rules"),
                                                        ".", tr("Automaton Rules file (*.atr"));
        AutomateHandler::getAutomateHandler().getAutomate(m_tabs->currentIndex())->saveRules(ruleFileName+".atr");
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
            this, SLOT(receiveCellHandler(QVector<uint>,CellHandler::generationTypes,uint,uint)));
    window->show();
}

/** \fn MainWindow::receiveCellHandler(const QVector<unsigned int> dimensions,
                                CellHandler::generationTypes type,
                                unsigned int stateMax, unsigned int density)
 * \brief Creates a new cellHandler with the provided arguments and updates the board with the created cellHandler
 */

void MainWindow::receiveCellHandler(const QVector<unsigned int> dimensions,
                                CellHandler::generationTypes type,
                                unsigned int stateMax, unsigned int density){
    AutomateHandler::getAutomateHandler().addAutomate(new Automate(dimensions, type, stateMax, density));

    if(m_tabs == NULL) createTabs();
    QWidget* newTab = createTab();
    m_tabs->addTab(newTab, "Automaton "+ QString::number(AutomateHandler::getAutomateHandler().getNumberAutomates()));
    m_tabs->setCurrentWidget(newTab);
    updateBoard(AutomateHandler::getAutomateHandler().getNumberAutomates()-1);

    RuleEditor* ruleEditor = new RuleEditor();
    connect(ruleEditor, SIGNAL(fileImported(QString)),this,SLOT(addAutomatonRuleFile(QString)));
    connect(ruleEditor, SIGNAL(rulesFilled(QList<const Rule*>)), this, SLOT(addAutomatonRules(QList<const Rule*>)));
    ruleEditor->show();

}

/** \fn MainWindow::nextState(int n)
 * \brief Shows the nth next state of the automaton on the board
 */

void MainWindow::nextState(unsigned int n){
    if(AutomateHandler::getAutomateHandler().getNumberAutomates()== 0){
        QMessageBox msgBox;
        msgBox.critical(0,"Error","Please create or import an Automaton first !");
        msgBox.setFixedSize(500,200);
    }
    else{

        AutomateHandler::getAutomateHandler().getAutomate(m_tabs->currentIndex())->run(n);
        updateBoard(m_tabs->currentIndex());
    }
}

/** \fn MainWindow::updateBoard()
 * \brief Updates cells on the board on the tab at the given index with the cellHandler's cells states
 */

void MainWindow::updateBoard(int index){
    if(AutomateHandler::getAutomateHandler().getNumberAutomates()== 0){
        QMessageBox msgBox;
        msgBox.critical(0,"Error","Please create or import an Automaton first !");
        msgBox.setFixedSize(500,200);
    }
    else{

        const CellHandler* cellHandler = &(AutomateHandler::getAutomateHandler().getAutomate(index)->getCellHandler());
        QVector<unsigned int> dimensions = cellHandler->getDimensions();
        QTableWidget* board = getBoard(index);
        if(dimensions.size() > 1){
            int i = 0;
            int j = 0;
            for (CellHandler::const_iterator it = cellHandler->begin(); it != cellHandler->end() && it.changedDimension() < 2; ++it){
                    if(it.changedDimension() > 0){
                        i = 0;
                        j++;
                    }
                    board->item(i,j)->setBackgroundColor(getColor(it->getState()));
                    i++;
            }
        }
        else{ // dimension = 1
            if (board->rowCount() != 1)
                addEmptyRow(index);
            int i = board->rowCount() -1;
            int j = 0;
            for (CellHandler::const_iterator it = cellHandler->begin(); it != cellHandler->end() && it.changedDimension() < 1; ++it){
                    board->item(i,j)->setBackgroundColor(getColor(it->getState()));
                    j++;
            }
            if (board->rowCount() == 1)
                addEmptyRow(index);

        }

    }

}

/** \fn MainWindow::forward()
 * \brief Show the Automaton's next state
 */

void MainWindow::forward(){
    nextState(1);
}

/** \fn MainWindow::getBoard()
 * \brief Returns the board of the n-th tab
 */
QTableWidget* MainWindow::getBoard(int n){
    return m_tabs->widget(n)->findChild<QTableWidget *>();
}

/** \brief Return the color wich correspond to the cellState
 */
QColor MainWindow::getColor(unsigned int cellState)
{
    if (cellState > QColor::colorNames().size() -2)
        return Qt::black;
    if (cellState == 0)
        return Qt::white;
    if (cellState == 1)
        return Qt::black;

    return QColor((Qt::GlobalColor)(cellState +2));
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
    connect(m_tabs, SIGNAL(currentChanged(int)), this, SLOT(handleTabChanged()));
}

/** \brief Add an empty row at the end of the board
 *
 * Used only in case of 1 dimension automaton
 *
 * \param n Index of the board
 */
void MainWindow::addEmptyRow(unsigned int n)
{
    QTableWidget *board = getBoard(n);
    board->setFixedHeight(board->height() + m_cellSize);
    unsigned int row = board->rowCount();
    board->insertRow(row);
    board->setRowHeight(row, m_cellSize);
    for(unsigned int col = 0; col < board->columnCount(); ++col) {
        board->setItem(row, col, new QTableWidgetItem(""));
        board->item(row, col)->setBackgroundColor("white");
        board->item(row, col)->setTextColor("black");
    }
}

/** \fn MainWindow::closeTab(int n)
 * \brief Closes the tab at index n. Before closing, prompts the user to save the automaton
 */

void MainWindow::closeTab(int n){
    m_tabs->setCurrentIndex(n);
    saveToFile();
    AutomateHandler::getAutomateHandler().deleteAutomate(AutomateHandler::getAutomateHandler().getAutomate(n));
    m_tabs->removeTab(n);
}

/** \fn MainWindow::addAutomatonRules(QList<const Rule *> rules)
 * \brief Adds a list of rules to the last Automaton
 */

void MainWindow::addAutomatonRules(QList<const Rule *> rules){
    for(int i =0 ; i < rules.size();i++)
    {
        AutomateHandler::getAutomateHandler().getAutomate(AutomateHandler::getAutomateHandler().getNumberAutomates()-1)->addRule(rules.at(i));
    }
}

/** \fn MainWindow::addAutomatonRuleFile(QString path)
 * \brief Adds a list of rules to the last Automaton from a given file
 */

void MainWindow::addAutomatonRuleFile(QString path){
    AutomateHandler::getAutomateHandler().getAutomate(AutomateHandler::getAutomateHandler().getNumberAutomates()-1)->addRuleFile(path);
}

/** \fn MainWindow::handlePlayPause()
 * \brief Handles the press event of the play/pause button
 */

void MainWindow::handlePlayPause(){
    if(AutomateHandler::getAutomateHandler().getNumberAutomates()== 0){
        QMessageBox msgBox;
        msgBox.critical(0,"Error","Please create or import an Automaton first !");
        msgBox.setFixedSize(500,200);
    }
    else{
        if(running){
            m_playPauseBt->setIcon(m_playIcon);
            delete m_timer;
        }
        else {
            m_playPauseBt->setIcon(m_pauseIcon);
            m_timer = new QTimer(this);
            connect(m_timer, SIGNAL(timeout()), this, SLOT(runAutomaton()));
            m_timer->start(m_timeStep->value());
        }
        running = !running;
    }


}

/** \fn MainWindow::runAutomaton()
 * \brief Runs the automaton simulation. Displays a new state on the board at regular intervals, set by the user in the interface.
 */

void MainWindow::runAutomaton(){
    if(running){
        AutomateHandler::getAutomateHandler().getAutomate(m_tabs->currentIndex())->run();
        QCoreApplication::processEvents();
        updateBoard(m_tabs->currentIndex());
        QCoreApplication::processEvents();
    }
}

/** \fn MainWindow::reset()
 * \brief Resets the current Automaton, by setting its cells to their initial state.
 */
void MainWindow::reset(){
    if(AutomateHandler::getAutomateHandler().getNumberAutomates()== 0){
        QMessageBox msgBox;
        msgBox.critical(0,"Error","Please create or import an Automaton first !");
        msgBox.setFixedSize(500,200);
    }
    else{
        //QTableWidget *board = getBoard(m_tabs->currentIndex());
        //board->setRowCount(1);
        //board->setFixedHeight(m_cellSize);

        AutomateHandler::getAutomateHandler().getAutomate(m_tabs->currentIndex())->getCellHandler().reset();
        updateBoard(m_tabs->currentIndex());
    }
}


/** \fn MainWindow::backward()
 * \brief Show the Automaton's previous state
 */

void MainWindow::backward(){
    AutomateHandler::getAutomateHandler().getAutomate(m_tabs->currentIndex())->getCellHandler().previousStates();
    updateBoard(m_tabs->currentIndex());
}

/** \fn MainWindow::cellPressed(int i, int j)
 * \brief Handles board cell press event
 */

void MainWindow::cellPressed(int i, int j){
    QVector<unsigned int> coord;

    m_currentCellX = i;
    m_currentCellY = j;
    const CellHandler* cellHandler = &(AutomateHandler::getAutomateHandler().getAutomate(m_tabs->currentIndex())->getCellHandler());
    if(cellHandler->getDimensions().size() > 1){
        coord.append(i);
        coord.append(j);
        m_cellSetter->setValue(cellHandler->getCell(coord)->getState());
    }
    else{
        coord.append(j);
        m_cellSetter->setValue(cellHandler->getCell(coord)->getState());
    }

    std::cout << "Current cell X" << m_currentCellX << std::endl;
    std::cout << "Current cell Y" << m_currentCellY << std::endl;
}


/** \fn MainWindow::changeCellValue()
 * \brief Sets the selected cell's value to the one set by the user.
 */

void MainWindow::changeCellValue(){
    if(AutomateHandler::getAutomateHandler().getNumberAutomates()== 0){
        QMessageBox msgBox;
        msgBox.critical(0,"Error","Please create or import an Automaton first !");
        msgBox.setFixedSize(500,200);
    }
    else{
        if(m_currentCellX > -1 && m_currentCellY > -1){
            const CellHandler* cellHandler = &(AutomateHandler::getAutomateHandler().getAutomate(m_tabs->currentIndex())->getCellHandler());
            QVector<unsigned int> coord;
            if(cellHandler->getDimensions().size() > 1){
                coord.append(m_currentCellX);
                coord.append(m_currentCellY);
                cellHandler->getCell(coord)->forceState(m_cellSetter->value());
            }
            else{
                coord.append(m_currentCellY);
                cellHandler->getCell(coord)->forceState(m_cellSetter->value());
            }


            updateBoard(m_tabs->currentIndex());
        }
    }
}

/** \fn MainWindow::handleTabChanged()
 * \brief Handles tab change
 */

void MainWindow::handleTabChanged(){
    if(m_tabs->currentIndex() >= 0){
        m_cellSetter->setMaximum(CellHandler::getMaxState());
        m_currentCellX = -1;
        m_currentCellY = -1;
    }

}


void MainWindow::setSize(int newCellSize)
{
    m_cellSize = newCellSize;
    if(AutomateHandler::getAutomateHandler().getNumberAutomates()!= 0)
    {
        for (unsigned int i = 0; i < m_tabs->count(); i++)
        {
            QTableWidget* board = getBoard(i);
            if (m_cellSize < 10)
                board->setShowGrid(false);
            else
                board->setShowGrid(true);
            for (unsigned int row = 0; row < board->rowCount(); row++)
                board->setRowHeight(row, m_cellSize);
            for (unsigned int col = 0; col < board->columnCount(); col++)
                board->setColumnWidth(col, m_cellSize);
            board->setFixedSize(board->columnCount()*m_cellSize, board->rowCount()*m_cellSize);
        }
    }
}

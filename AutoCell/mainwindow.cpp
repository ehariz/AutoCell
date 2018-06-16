#include "mainwindow.h"
#include <iostream>
#include "math.h"

/** \brief Constructor of the main window
 */
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    createButtons();
    createToolBar();


    setMinimumSize(500,500);
    setWindowTitle("AutoCell");

    m_tabs = NULL;
    m_running = false;

    QSettings settings;
    int nbAutomate = settings.value("nbAutomate").toInt();
    for (int i = 0; i < nbAutomate; i++)
    {
        QString fileName = QString(".automate"+QString::number(i));
        try{
            AutomateHandler::getAutomateHandler().addAutomate(new Automate(QString(fileName+".atc"), QString(fileName+".atr")));
        if(m_tabs == NULL)
            createTabs();
        m_tabs->addTab(createTab(), "Automaton "+ QString::number(AutomateHandler::getAutomateHandler().getNumberAutomates()));
        updateBoard(AutomateHandler::getAutomateHandler().getNumberAutomates()-1);
        }
        catch (QString &s)
        {
            QMessageBox msgBox;
            msgBox.warning(0,"Error",s);
            msgBox.setFixedSize(500,200);
        }
        QFile fichier(QString(fileName + ".atc"));
        fichier.remove();
        fichier.close();
        QFile fichier2(QString(fileName + ".atr"));
        fichier2.remove();
    }
    m_zoom->setValue(settings.value("zoom").toInt());
    m_timeStep->setValue(settings.value("timestamp").toInt());
}

/** \brief Destructor of the main window
 *
 * It is here that the settings are saved
 */
MainWindow::~MainWindow()
{
    // Saving settings for further sessions
    QSettings settings;
    settings.setValue("nbAutomate", AutomateHandler::getAutomateHandler().getNumberAutomates());
    settings.setValue("zoom", m_zoom->value());
    settings.setValue("timestamp", m_timeStep->value());

    for (unsigned int i = 0; i < AutomateHandler::getAutomateHandler().getNumberAutomates(); i++)
    {
        AutomateHandler::getAutomateHandler().getAutomate(i)->saveAll(QString(".automate"+QString::number(i)+".atc"), QString(".automate"+QString::number(i)+".atr"));
    }

}


/** \fn MainWindow::createButtons()
 * \brief Creates and connects buttons for the MainWindow
 */

void MainWindow::createButtons(){

    QPixmap previousStatePm(":/icons/icons/fast-backward.svg");
    QPixmap previousStateHoveredPm(":/icons/icons/fast-backward-full.svg");
    QPixmap nextStatePm(":/icons/icons/fast-forward.svg");
    QPixmap nextStateHoveredPm(":/icons/icons/fast-forward-full.svg");
    QPixmap playPm(":/icons/icons/play.svg");
    QPixmap playHoveredPm(":/icons/icons/play-full.svg");
    QPixmap newPm(":/icons/icons/new.svg");
    QPixmap openPm(":/icons/icons/open.svg");
    QPixmap savePm(":/icons/icons/save.svg");
    QPixmap pausePm(":/icons/icons/pause.svg");
    QPixmap resetPm(":/icons/icons/reset.svg");

    QIcon previousStateIcon;
    QIcon nextStateIcon;
    QIcon newIcon;
    QIcon saveIcon;
    QIcon openIcon;
    QIcon resetIcon;

    previousStateIcon.addPixmap(previousStatePm, QIcon::Normal, QIcon::Off);
    previousStateIcon.addPixmap(previousStateHoveredPm, QIcon::Active, QIcon::Off);
    nextStateIcon.addPixmap(nextStatePm, QIcon::Normal, QIcon::Off);
    nextStateIcon.addPixmap(nextStateHoveredPm, QIcon::Active, QIcon::Off);
    m_playIcon.addPixmap(playPm, QIcon::Normal, QIcon::Off);
    m_playIcon.addPixmap(playHoveredPm, QIcon::Active, QIcon::Off);
    m_pauseIcon.addPixmap(pausePm, QIcon::Normal, QIcon::Off);
    newIcon.addPixmap(newPm, QIcon::Normal, QIcon::Off);
    saveIcon.addPixmap(savePm, QIcon::Normal, QIcon::Off);
    openIcon.addPixmap(openPm, QIcon::Normal, QIcon::Off);
    resetIcon.addPixmap(resetPm, QIcon::Normal, QIcon::Off);

    QAction *playPause = new QAction(m_playIcon, tr("Play"), this);
    QAction *nextState = new QAction(nextStateIcon, tr("&Next state"), this);
    QAction *previousState = new QAction(previousStateIcon, tr("&Previous state"), this);
    QAction *openAutomaton = new QAction(openIcon, tr("Open automaton"), this);
    QAction *saveAutomaton = new QAction(saveIcon, tr("Save automaton"), this);
    QAction *newAutomaton = new QAction(newIcon, tr("New automaton"), this);
    QAction *resetAutomaton = new QAction(resetIcon, tr("Reset automaton"), this);

    m_previousStateBt = new QToolButton(this);
    m_nextStateBt = new QToolButton(this);
    m_playPauseBt = new QToolButton(this);
    m_saveAutomatonBt = new QToolButton(this);
    m_newAutomatonBt = new QToolButton(this);
    m_openAutomatonBt = new QToolButton(this);
    m_resetBt = new QToolButton(this);

    m_previousStateBt->setDefaultAction(previousState);
    m_nextStateBt->setDefaultAction(nextState);
    m_playPauseBt->setDefaultAction(playPause);
    m_saveAutomatonBt->setDefaultAction(saveAutomaton);
    m_newAutomatonBt->setDefaultAction(newAutomaton);
    m_openAutomatonBt->setDefaultAction(openAutomaton);
    m_resetBt->setDefaultAction(resetAutomaton);

    m_previousStateBt->setIconSize(QSize(30,30));
    m_nextStateBt->setIconSize(QSize(30,30));
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
    connect(m_nextStateBt, SIGNAL(clicked(bool)), this, SLOT(forward()));
    connect(m_previousStateBt, SIGNAL(clicked(bool)), this, SLOT(backward()));
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
    tbLayout->addWidget(m_previousStateBt, Qt::AlignCenter);
    tbLayout->addWidget(m_playPauseBt, Qt::AlignCenter);
    tbLayout->addWidget(m_nextStateBt, Qt::AlignCenter);
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
        for(int col = 0; col < boardHSize; ++col)
            board->setColumnWidth(col, m_cellSize);
        for(int row = 0; row < boardVSize; ++row) {
            board->setRowHeight(row, m_cellSize);
            for(int col = 0; col < boardHSize; ++col) {
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

        RuleEditor* ruleEditor = new RuleEditor(AutomateHandler::getAutomateHandler().getAutomate(AutomateHandler::getAutomateHandler().getNumberAutomates()-1)->getCellHandler().getDimensions().size(), this);
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

    RuleEditor* ruleEditor = new RuleEditor(AutomateHandler::getAutomateHandler().getAutomate(AutomateHandler::getAutomateHandler().getNumberAutomates()-1)->getCellHandler().getDimensions().size(), this);
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

            // Go to bottom
            QScrollArea *scrool = static_cast<QScrollArea*>(m_tabs->currentWidget()->layout()->itemAt(0)->widget());

            scrool->verticalScrollBar()->setSliderPosition(scrool->verticalScrollBar()->maximum());

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
 *
 * The maximal state supported is 21. Use hookMoreColor to add more.
 */
QColor MainWindow::getColor(int cellState)
{
    if (cellState > QColor::colorNames().size() -2)
        return Qt::black;
    switch (cellState)
    {
    case 0:
        return Qt::white;
    case 1:
        return Qt::black;
    case 2:
        return Qt::red;
    case 3:
        return Qt::green;
    case 4:
        return Qt::blue;
    case 5:
        return Qt::yellow;
    case 6:
        return QColor(170, 110, 40); // brown
    case 7:
        return QColor(145,30, 180); // purple
    case 8:
        return QColor(245,130,48); // orange
    case 9:
        return Qt::cyan;
    case 10:
        return Qt::magenta;
    case 11:
        return QColor(210, 245, 60); // Lime
    case 12:
        return QColor(250, 190, 190); // pink
    case 13:
        return QColor(0,128,128); // Teal
    case 14:
        return QColor(230, 190, 255); // Lavender
    case 15:
        return QColor(255, 250, 200); // beige
    case 16:
        return QColor(128, 0,0); // Maroon
    case 17:
        return QColor(170, 255, 195); // Mint
    case 18:
        return QColor(128, 128, 0); // Olive
    case 19:
        return QColor(255, 215, 180); // Coral
    case 20:
        return QColor(0,0,128); // Navy
    case 21:
        return Qt::gray;


    }

    return hookMoreColor(cellState);
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
    for(int col = 0; col < board->columnCount(); ++col) {
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
        if(m_running){
            m_playPauseBt->setIcon(m_playIcon);
            delete m_timer;
        }
        else {
            m_playPauseBt->setIcon(m_pauseIcon);
            m_timer = new QTimer(this);
            connect(m_timer, SIGNAL(timeout()), this, SLOT(runAutomaton()));
            m_timer->start(m_timeStep->value());
        }
        m_running = !m_running;
    }


}

/** \fn MainWindow::runAutomaton()
 * \brief Runs the automaton simulation. Displays a new state on the board at regular intervals, set by the user in the interface.
 */

void MainWindow::runAutomaton(){
    if(m_running){
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
        if (AutomateHandler::getAutomateHandler().getAutomate(m_tabs->currentIndex())->getCellHandler().getDimensions().size() == 1)
        {
            QTableWidget *board  = getBoard(m_tabs->currentIndex());
            board->setRowCount(0);
            board->setFixedHeight(0);
        }
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
                updateBoard(m_tabs->currentIndex());
            }
            else{
                coord.append(m_currentCellY);
                cellHandler->getCell(coord)->forceState(m_cellSetter->value());
                QTableWidget *board = getBoard(m_tabs->currentIndex());
                int i = 0;
                int j = 0;
                for (CellHandler::const_iterator it = cellHandler->begin(); it != cellHandler->end() && it.changedDimension() < 1; ++it){
                        board->item(i,j)->setBackgroundColor(getColor(it->getState()));
                        j++;
                }
            }

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
        if(m_running){
            m_playPauseBt->setIcon(m_playIcon);
            delete m_timer;
            m_running = !m_running;
        }
    }

}

/** \brief Change the size of the board
 * \param newCellSize New Cell size
 */
void MainWindow::setSize(int newCellSize)
{
    m_cellSize = newCellSize;
    if(AutomateHandler::getAutomateHandler().getNumberAutomates()!= 0)
    {
        for (int i = 0; i < m_tabs->count(); i++)
        {
            QTableWidget* board = getBoard(i);
            if (m_cellSize < 10)
                board->setShowGrid(false);
            else
                board->setShowGrid(true);
            for (int row = 0; row < board->rowCount(); row++)
                board->setRowHeight(row, m_cellSize);
            for (int col = 0; col < board->columnCount(); col++)
                board->setColumnWidth(col, m_cellSize);
            board->setFixedSize(board->columnCount()*m_cellSize, board->rowCount()*m_cellSize);
        }
    }
}

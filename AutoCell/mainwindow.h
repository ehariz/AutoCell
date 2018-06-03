#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include "cellhandler.h"
#include "creationdialog.h"


/** \class MainWindow
 * \brief Simulation window
 *
 * Displays the automaton's current state as a board and contains user interaction components.
 */

class MainWindow : public QMainWindow
{
    Q_OBJECT

    QTabWidget *m_tabs; //Tabs for the main window
    QVector<CellHandler *> m_cellHandlers; //QVector containing each tab's cellHandler

    ///Icons
    QIcon m_fastBackwardIcon;
    QIcon m_fastForwardIcon;
    QIcon m_playIcon;
    QIcon m_pauseIcon;
    QIcon m_newIcon;
    QIcon m_saveIcon;
    QIcon m_openIcon;
    QIcon m_resetIcon;

    ///Actions
    QAction *m_playPause;
    QAction *m_nextState;
    QAction *m_previousState;
    QAction *m_fastForward;
    QAction *m_fastBackward;
    QAction *m_openAutomaton;
    QAction *m_saveAutomaton;
    QAction *m_newAutomaton;
    QAction *m_resetAutomaton;

    ///Buttons
    QToolButton *m_playPauseBt;
    QToolButton *m_nextStateBt;
    QToolButton *m_previousStateBt;
    QToolButton *m_fastForwardBt;
    QToolButton *m_fastBackwardBt;
    QToolButton *m_openAutomatonBt;
    QToolButton *m_saveAutomatonBt;
    QToolButton *m_newAutomatonBt;
    QToolButton *m_resetBt;


    QSpinBox *m_jumpSpeed; ///Simulation speed input
    QLabel *m_speedLabel;

    QToolBar *m_toolBar; ///Toolbar containing the buttons

    ///Board size settings
    unsigned int m_boardHSize = 25;
    unsigned int m_boardVSize = 25;
    unsigned int m_cellSize = 30;

    void createIcons();
    void createActions();
    void createToolBar();
    void createBoard();
    QWidget* createTab();
    void createTabs();


    void updateBoard(int index);
    void nextState(int n);
    QTableWidget* getBoard(int n);


public:
    explicit MainWindow(QWidget *parent = nullptr);


signals:

public slots:
    void openFile();
    void saveToFile();
    void openCreationWindow();
    void setCellHandler(const QVector<unsigned int> dimensions,
                        CellHandler::generationTypes type = CellHandler::generationTypes::empty,
                        unsigned int stateMax = 1, unsigned int density = 20);
    void forward();
    void closeTab(int n);

};

#endif // MAINWINDOW_H

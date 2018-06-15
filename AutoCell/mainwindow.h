#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include "cellhandler.h"
#include "automate.h"
#include "creationdialog.h"
#include "automatehandler.h"
#include "ruleeditor.h"

/** \class MainWindow
 * \brief Simulation window
 *
 * Displays the automaton's current state as a board and contains user interaction components.
 */

class MainWindow : public QMainWindow
{
    Q_OBJECT

    QTabWidget *m_tabs; ///Tabs for the main window

    ///Icons saved for reuse
    QIcon m_playIcon;
    QIcon m_pauseIcon;

    ///Buttons
    QToolButton *m_playPauseBt;
    QToolButton *m_nextStateBt;
    QToolButton *m_previousStateBt;
    QToolButton *m_openAutomatonBt;
    QToolButton *m_saveAutomatonBt;
    QToolButton *m_newAutomatonBt;
    QToolButton *m_resetBt;


    QSpinBox *m_timeStep; ///Simulation time step duration input
    QSpinBox *m_cellSetter; ///Cell state manual modification
    QTimer* m_timer; /// Timer running between simulation steps

    QSlider *m_zoom;

    bool running;
    QToolBar *m_toolBar; ///Toolbar containing the buttons

    int m_currentCellX;
    int m_currentCellY;

    ///Board size settings
    unsigned int m_boardHSize = 25;
    unsigned int m_boardVSize = 25;
    unsigned int m_cellSize = 30;

    void createButtons();
    void createToolBar();
    void createBoard();
    QWidget* createTab();
    void createTabs();

    void addEmptyRow(unsigned int n);
    void updateBoard(int index);
    void nextState(unsigned int n);
    QTableWidget* getBoard(int n);

    static QColor getColor(unsigned int cellState);


public:
    explicit MainWindow(QWidget *parent = nullptr);
    virtual ~MainWindow();

signals:

public slots:
    void openFile();
    void saveToFile();
    void openCreationWindow();
    void receiveCellHandler(const QVector<unsigned int> dimensions,
                        CellHandler::generationTypes type = CellHandler::generationTypes::empty,
                        unsigned int stateMax = 1, unsigned int density = 20);
    void addAutomatonRules(QList<const Rule *> rules);
    void addAutomatonRuleFile(QString path);
    void forward();
    void backward();
    void closeTab(int n);
    void runAutomaton();
    void handlePlayPause();
    void reset();
    void cellPressed(int i, int j);
    void changeCellValue();
    void handleTabChanged();
    void setSize(int newCellSize);

};

#endif // MAINWINDOW_H

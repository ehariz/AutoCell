#ifndef CREATIONDIALOG_H
#define CREATIONDIALOG_H

#include <QtWidgets>
#include "cellhandler.h"

/** \class CreationDialog
 * \brief Automaton creation dialog box
 *
 * Allow the user to input settings to create an automaton
 */

class CreationDialog : public QDialog
{
    Q_OBJECT

public:
    CreationDialog(QWidget *parent = 0);

signals:
    void settingsFilled(const QVector<unsigned int> dimensions,
                        CellHandler::generationTypes type = CellHandler::generationTypes::empty,
                        unsigned int stateMax = 1, unsigned int density = 20);

public slots:
    void processSettings();

private:
    QLineEdit *m_dimensionsEdit;
    QSpinBox *m_densityBox;
    QSpinBox *m_stateMaxBox;
    QPushButton *m_doneBt;

    QGroupBox *m_groupBox;
    QRadioButton *m_empGen;
    QRadioButton *m_randGen;
    QRadioButton *m_symGen;

    QGroupBox *createGenButtons();






};

#endif // CREATEDIALOG_H

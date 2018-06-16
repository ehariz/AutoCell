#ifndef RULEEDITOR_H
#define RULEEDITOR_H
#include <QtWidgets>
#include "neighbourrule.h"
#include "automate.h"

/** \brief Dialog for editing the rules
 */
class RuleEditor : public QDialog
{
    Q_OBJECT
    QList<const Rule*> m_rules;
    QListWidget* m_rulesListWidget;
    QTableWidget* m_rulesTable;

    QSpinBox* m_outputStateBox;
    QLineEdit* m_currentStatesEdit;
    QLineEdit* m_neighbourStatesEdit;
    QSpinBox* m_upperNeighbourBox;
    QSpinBox* m_lowerNeighbourBox;
    QSpinBox* m_automatonNumber;

    QPushButton* m_addBt;
    QPushButton* m_doneBt;
    QPushButton* m_removeBt;
    QPushButton* m_importBt;

    unsigned int m_selectedRule;
    unsigned int m_dimensions; ///< Dimensions of the automaton, to manage 1D dimensions


public:
    explicit RuleEditor(unsigned int dimensions, QWidget *parent = nullptr);

signals:
    void rulesFilled(QList<const Rule*> rules);
    void fileImported(QString path);

public slots:
    void removeRule();
    void addRule();
    void importFile();
    void sendRules();


};

#endif // RULEEDITOR_H

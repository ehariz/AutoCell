#ifndef RULE_H
#define RULE_H

#include <QPair>
#include <QVector>
#include "cell.h"

class Rule ///< Rule modifying a cell depending on the number of its neighbours belonging to a range
{
private:
    QVector<unsigned int> m_currentCellPossibleValues;
    QPair<unsigned int , unsigned int> m_neighbourInterval; ///< Stores the rule condition regarding the number of neighbours
    QVector<unsigned int> m_neighbourPossibleValues; ///< Stores the possible values of the neighbours to fit with the rule
    //plutôt utiliser un QSet pour éviter les doublons
    unsigned int m_cellOutputState; ///< Stores the output state of the cell if the condition is matched
    bool inInterval(unsigned int i);
public:
    Rule(QVector<unsigned int> currentCellValues, QPair<unsigned int, unsigned int> intervalNumber, QVector<unsigned int> intervalValues):
        m_currentCellPossibleValues(currentCellValues), m_neighbourInterval(intervalNumber), m_neighbourPossibleValues(intervalValues);
    ~Rule();
    bool matchCell(const Cell & c)const;
    unsigned int getCellOutputState()const;
};

#endif // RULE_H

#ifndef RULE_H
#define RULE_H

#include <QPair>
#include <QVector>
#include <QSet>
#include <QJsonDocument>
#include "cell.h"

/** \class Rule
 * \brief Contains the rule condition and the output state if that condition is satisfied
 * The rule modifies a cell depending on the number of its neighbours belonging to a range
 */
class Rule
{
private:
    QVector<unsigned int> m_currentCellPossibleValues; ///< Stores the possible values of the current cell as part of the rule condition
    QPair<unsigned int , unsigned int> m_neighbourInterval; ///< Stores the rule condition regarding the number of neighbours
    //ATTENTION check that first is lower than second
    QSet<unsigned int> m_neighbourPossibleValues; ///< Stores the possible values of the neighbours to fit with the rule
    unsigned int m_cellOutputState; ///< Stores the output state of the cell if the condition is matched
    bool inInterval(unsigned int matchingNeighbours)const; ///< Checks if the number of neighbours matching the state condition belongs to the condition interval
    //bool load(const QJsonObject &json);
public:
    Rule(QVector<unsigned int> currentCellValues, QPair<unsigned int, unsigned int> intervalNumber, QSet<unsigned int> intervalValues, unsigned int outputState);
    ~Rule();
    bool matchCell(const Cell & c)const;
    unsigned int getCellOutputState()const;
};

#endif // RULE_H

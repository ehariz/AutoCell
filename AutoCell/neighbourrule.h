#ifndef NEIGHBOURRULE_H
#define NEIGHBOURRULE_H

#include <QPair>
#include <QSet>
#include "cell.h"
#include "rule.h"

/** \class NeighbourRule
 * \brief Contains the rule condition and the output state if that condition is satisfied
 * The rule modifies a cell depending on the number of its neighbours belonging to a range
 */
class NeighbourRule : public Rule
{
private:
    QPair<unsigned int , unsigned int> m_neighbourInterval; ///< Stores the rule condition regarding the number of neighbours
    //ATTENTION check that first is lower than second
    QSet<unsigned int> m_neighbourPossibleValues; ///< Stores the possible values of the neighbours to fit with the rule
    bool inInterval(unsigned int matchingNeighbours)const;
    //bool load(const QJsonObject &json);
public:
    NeighbourRule(QVector<unsigned int> currentCellValues, QPair<unsigned int, unsigned int> intervalNumber, QSet<unsigned int> intervalValues, unsigned int outputState);
    ~NeighbourRule();
    bool matchCell(Cell * c)const;
};

#endif // NEIGHBOURRULE_H

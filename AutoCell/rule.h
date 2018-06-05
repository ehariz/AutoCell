#ifndef RULE_H
#define RULE_H

#include <QVector>
#include "cell.h"

/** \class Rule
 * \brief
 */
class Rule
{
protected:
    QVector<unsigned int> m_currentCellPossibleValues; ///< Stores the possible values of the current cell as part of the rule condition
    unsigned int m_cellOutputState; ///< Stores the output state of the cell if the condition is matched
public:
    Rule(QVector<unsigned int> currentCellValues, unsigned int outputState);

    /** \brief Verify if the cell match the rule
     *
     * Using :
     * \code
     * if (rule.matchCell(&cell))
     *     cell.setState(rule.getCellOutputState());
     * \endcode
     * \param c Cell to test
     */
    virtual bool matchCell(const Cell * c)const = 0;
    unsigned int getCellOutputState() const;
};

#endif // RULE_H

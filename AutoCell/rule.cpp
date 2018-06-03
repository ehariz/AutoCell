#include "rule.h"

Rule::Rule(QVector<unsigned int> currentCellValues, unsigned int outputState):
        m_currentCellPossibleValues(currentCellValues), m_cellOutputState(outputState)
{

}

/** \brief Get the rule output state that will be the next state if the cell matches the rule condition
 */
unsigned int Rule::getCellOutputState()const
{
        return m_cellOutputState;
}

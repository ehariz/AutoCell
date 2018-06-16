#include "rule.h"

/** \brief Constructor of Rule
 * \param currentCellValues List of possibles values for the current cell
 * \param outputState Next cell state
 */
Rule::Rule(QVector<unsigned int> currentCellValues, unsigned int outputState):
        m_currentCellPossibleValues(currentCellValues), m_cellOutputState(outputState)
{

}

/** \brief Create a QJsonObject for the current rule
 */
QJsonObject Rule::toJson() const
{
    QJsonObject object;
    object.insert("finalState", QJsonValue((int)m_cellOutputState));

    QJsonArray currentStates;
    for (unsigned int i = 0; i < m_currentCellPossibleValues.size(); i++)
    {
        currentStates.append(QJsonValue((int)m_currentCellPossibleValues.at(i)));
    }
    object.insert("currentStates", currentStates);

    return object;
}

/** \brief Get the rule output state that will be the next state if the cell matches the rule condition
 */
unsigned int Rule::getCellOutputState()const
{
    return m_cellOutputState;
}


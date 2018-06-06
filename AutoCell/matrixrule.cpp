#include "matrixrule.h"

/** \brief Returns a vector fill of the integers between min and max (all included)
 * \return Interval
 * \param min Minimal value (included)
 * \param max Maximal value (included)
 */
QVector<unsigned int> fillInterval(unsigned int min, unsigned int max)
{
    QVector<unsigned int> interval;
    for (unsigned int i = min; i <= max ; i++)
        interval.push_back(i);

    return interval;
}

/** \brief Constructor
 * \param finalState Final state if the rule match the cell
 * \param currentStates Possibles states of the cell. Nothing means all states
 */
MatrixRule::MatrixRule(unsigned int finalState, QVector<unsigned int> currentStates) :
    Rule(currentStates, finalState)
{
}

/** \brief Tells if the cell match the rule
 * \param cell Cell to test
 * \return True if the cell match the rule
 */
bool MatrixRule::matchCell(const Cell *cell) const
{
    // Check cell state
    if (!m_currentCellPossibleValues.contains(cell->getState()))
    {
        return false;
    }

    // Check neighbours
    bool matched = true;
    // Rappel : QMap<relativePosition, possibleStates>
    for (QMap<QVector<short>,  QVector<unsigned int> >::const_iterator it = m_matrix.begin() ; it != m_matrix.end(); ++it)
    {
        if (cell->getNeighbour(it.key()) == nullptr) // Border management
        {
            matched = false;
            break;
        }
        if (! it.value().contains(cell->getNeighbour(it.key())->getState()))
        {
            matched = false;
            break;
        }
    }

    return matched;
}

/** \brief Add a possible state to a relative position
 */
void MatrixRule::addNeighbourState(QVector<short> relativePosition, unsigned int matchState)
{
    m_matrix[relativePosition].push_back(matchState);
}

/** \brief Add multiples possible states to existents states
 */
void MatrixRule::addNeighbourState(QVector<short> relativePosition, QVector<unsigned int> matchStates)
{
    for (QVector<unsigned int>::const_iterator it = matchStates.begin(); it != matchStates.end(); ++it)
        m_matrix[relativePosition].push_back(*it);
}

/** \brief Return a QJsonObject to save the rule
 */
QJsonObject MatrixRule::toJson() const
{
    QJsonObject object(Rule::toJson());

    object.insert("type", QJsonValue("matrix"));

    QJsonArray neighbours;
    for (QMap<QVector<short>,  QVector<unsigned int> >::const_iterator it = m_matrix.begin(); it != m_matrix.end(); ++it)
    {
        QJsonObject aNeighbour;
        QJsonArray relativePosition;
        for (unsigned int i = 0; i < it.key().size(); i++)
        {
            relativePosition.append(QJsonValue((int)it.key().at(i)));
        }
        aNeighbour.insert("relativePosition", relativePosition);

        QJsonArray neighbourStates;
        for (unsigned int i = 0; i < it.value().size(); i++)
        {
            neighbourStates.append(QJsonValue((int)it.value().at(i)));
        }
        aNeighbour.insert("neighbourStates", neighbourStates);

        neighbours.append(aNeighbour);
    }
    object.insert("neighbours", neighbours);

    return object;
}

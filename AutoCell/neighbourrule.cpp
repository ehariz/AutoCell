#include "neighbourrule.h"

/** \brief Checks if the number of neighbours matching the state condition belongs to the condition interval
 *
 * \param number of neighbours matching the rule condition regarding their values
 * \return True if the number of neighbours matches with the interval condition
 */
bool NeighbourRule::inInterval(unsigned int matchingNeighbours)const
{
    if(matchingNeighbours >= m_neighbourInterval.first && matchingNeighbours<=m_neighbourInterval.second)
        return true;
    else
        return false;
}

/** \brief Constructs a neighbour rule with the parameters.

 */
NeighbourRule::NeighbourRule(QVector<unsigned int> currentCellValues, QPair<unsigned int, unsigned int> intervalNumber, QSet<unsigned int> intervalValues, unsigned int outputState):
        Rule(currentCellValues, outputState), m_neighbourInterval(intervalNumber), m_neighbourPossibleValues(intervalValues)
{

}

NeighbourRule::~NeighbourRule()
{

}

/** \brief Checks if the input cell satisfies the rule condition.
 *
 *
 * \param c current cell
 * \return True if the cell matches the rule condition
 */
bool NeighbourRule::matchCell(Cell * c)const
{
    unsigned int matchingNeighbours = 0;
   // QSet<unsigned int> set;
    //QSet<unsigned int> m_neighbourPossibleValues;
    //set<<3<<2<<5<<9;
    QSet<unsigned int>::const_iterator i = m_neighbourPossibleValues.constBegin();
    while (i != m_neighbourPossibleValues.constEnd()) {
        //std::cout<<*i;
        matchingNeighbours += c->countNeighbours(*i);
        ++i;
    }
    if(m_currentCellPossibleValues.contains(c->getState()) && inInterval(matchingNeighbours))
        return true; //the rule can be applied to the cell
    else
        return false; //the rule cannot be applied to the cell
}

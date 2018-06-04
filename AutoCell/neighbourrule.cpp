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
NeighbourRule::NeighbourRule(unsigned int outputState, QVector<unsigned int> currentCellValues, QPair<unsigned int, unsigned int> intervalNbrNeighbour, QSet<unsigned int> neighbourValues) :
        Rule(currentCellValues, outputState), m_neighbourInterval(intervalNbrNeighbour), m_neighbourPossibleValues(neighbourValues)
{
    if (m_neighbourInterval.first == 0)
        throw QString(QObject::tr("Low value of the number of neighbour interval can't be 0"));
    if (m_neighbourInterval.first > m_neighbourInterval.second)
        throw QString(QObject::tr("The interval must be (x,y) with x <= y"));
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
bool NeighbourRule::matchCell(const Cell *c)const
{
    unsigned int matchingNeighbours = 0;

    if (!m_currentCellPossibleValues.contains(c->getState()))
        return false;

   // QSet<unsigned int> set;
    //QSet<unsigned int> m_neighbourPossibleValues;
    //set<<3<<2<<5<<9;
    QSet<unsigned int>::const_iterator i = m_neighbourPossibleValues.constBegin();
    if (i == m_neighbourPossibleValues.constEnd()) // All possibles values (except 0)
    {
        matchingNeighbours = c->countNeighbours();
    }
    else
    {
        while (i != m_neighbourPossibleValues.constEnd()) {
            //std::cout<<*i;
            matchingNeighbours += c->countNeighbours(*i);
            ++i;
        }
    }
    if(!inInterval(matchingNeighbours))
        return false; //the rule cannot be applied to the cell

    return true; //the rule can be applied to the cell

}

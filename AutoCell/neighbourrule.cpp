#include "neighbourrule.h"

/** \brief Recursive function which browse the position possibilities tree
 *
 * The game of life by John Horton Conway:
 *
 * "At each step, the cell evolution is determined by the state of its 8 neighbours as following:
 * A dead cell which has exactly 3 living neighbours starts to live.
 * An alive cell which has 2 or 3 living neighbours stays alive, else it dies."
 *
 * 1 : cell is alive
 * 0 : cell is dead
 *
 * \code
 * Rule 1: dead cell (state 0) starts living (state 1) if it has exactly 3 living neighbours (in state 1)
 *
 * unsigned int rule1OutputState = 1; // output state is alive state
 *
 * QVector<unsigned int> rule1CurrentCellValues;
 * rule1CurrentCellValues.insert(0); //current cell is dead
 *
 * QPair<unsigned int, unsigned int> rule1intervalNbrNeighbours;
 * rule1IntervalNbrNeighbours.first = 3;
 * rule1IntervalNbrNeighbours.second = 3;
 *
 * QSet<unsigned int> rule1NeighbourPossibleValues;
 * rule1NeighbourPossibleValues<<1; //living neighbours
 *
 * NeighbourRule rule1 = NeighbourRule(rule1OutputState, rule1CurrentCellValues, rule1IntervalNbrNeighbours, rule1NeighbourPossibleValues);
 *
 *
 *
 * Rule 2: alive cell (state 1) dies (goes to state 0) if it has 0 to 1 living neighbours (in state 1)
 *
 * unsigned int rule2OutputState = 0; // output state is dead state
 *
 * QVector<unsigned int> rule2CurrentCellValues;
 * rule2CurrentCellValues.insert(1); //current cell is alive
 *
 * QPair<unsigned int, unsigned int> rule2intervalNbrNeighbours;
 * rule2IntervalNbrNeighbours.first = 0;
 * rule2IntervalNbrNeighbours.second = 1;
 *
 * QSet<unsigned int> rule2NeighbourPossibleValues;
 * rule2NeighbourPossibleValues<<1; //living neighbours
 *
 * NeighbourRule rule2 = NeighbourRule(rule2OutputState, rule2CurrentCellValues, rule2IntervalNbrNeighbours, rule2NeighbourPossibleValues);
 *
 *
 *
 * Rule 3: alive cell (state 1) dies (goes to state 0) if it has 4 to 8 living neighbours (in state 1)
 *
 * unsigned int rule3OutputState = 0; // output state is dead state
 *
 * QVector<unsigned int> rule3CurrentCellValues;
 * rule2CurrentCellValues.insert(1); //current cell is alive
 *
 * QPair<unsigned int, unsigned int> rule3intervalNbrNeighbours;
 * rule3IntervalNbrNeighbours.first = 4;
 * rule3IntervalNbrNeighbours.second = 8;
 *
 * QSet<unsigned int> rule3NeighbourPossibleValues;
 * rule3NeighbourPossibleValues<<1; //living neighbours
 *
 * NeighbourRule rule3 = NeighbourRule(rule3OutputState, rule3CurrentCellValues, rule3IntervalNbrNeighbours, rule3NeighbourPossibleValues);
 *
 * \endcode
 */

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
    if (m_neighbourInterval.second == 0)
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

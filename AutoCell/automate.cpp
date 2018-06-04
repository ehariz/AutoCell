#include "automate.h"

Automate::Automate(CellHandler & cellHandler, const Rule & rule):
        m_cellHandler(cellHandler), m_rule(rule)
{
}

/** \brief Apply the rule on the cells grid nbSteps times
 *
 * \param nbSteps number of iterations of the automate on the cell grid
 * \param neighbour New neighbour
 * \return False if the neighbour already exists
 */
bool Automate::run(unsigned int nbSteps) //void instead ?
{
    for(unsigned int i = 0; i<nbSteps; ++i)
    {
        for (CellHandler::iterator it = m_cellHandler.begin(); it != m_cellHandler.end(); ++it)
        {
            if(m_rule.matchCell(*it)) //if the cell matches with the rule, its state is changed
            {
                it->setState(m_rule.getCellOutputState());
            }
        }
        m_cellHandler.nextStates(); //apply the changes to all the cells simultaneously
    }
    return true;

}

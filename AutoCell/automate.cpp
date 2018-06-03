#include "automate.h"

Automate::Automate(const CellHandler::CellHandler & cellHandler, const Rule::Rule & rule):
        m_cellHandler(cellHandler), m_rule(rule)
{
}

bool Automate::run(unsigned int nbSteps)
{ //balayage de la grille nbSteps fois
    for(unsigned int i = 0; i<nbSteps; ++i)
    {
        for (CellHandler::iterator it = m_cellHandler.begin(); it != m_cellHandler.end(); ++it)
        {
            if(m_rule.matchCell(*it)) //if the cell matches with the rule, its state is changed
            {
                it->setState(m_rule.getCellOutputState());
            }
            m_cellHandler.nextStates(); //apply the changes to all the cells "simultaneously"
        }
    }
}

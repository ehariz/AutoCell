#ifndef AUTOMATE_H
#define AUTOMATE_H

#include "cellhandler.h"
#include "rule.h"

/** \class Automate
 * \brief
 */
class Automate
{
private:
    CellHandler & m_cellHandler; ///< CellHandler to go through
    Rule & m_rule; ///< Rule to use on the cells
    friend class AutomateHandler;
    Automate(CellHandler & cellHandler, Rule & rule);
    //selectRule?
public:
    bool run(unsigned int nbSteps = 1);
};

#endif // AUTOMATE_H

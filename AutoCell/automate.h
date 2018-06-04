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
    const Rule & m_rule; ///< Rule to use on the cells
    friend class AutomateHandler;
public:
    Automate(CellHandler& cellHandler, const Rule & rule);
    //selectRule?
public:
    bool run(unsigned int nbSteps = 1);
};

#endif // AUTOMATE_H

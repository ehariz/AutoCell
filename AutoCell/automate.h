#ifndef AUTOMATE_H
#define AUTOMATE_H

#include "cellhandler.h"
#include "rule.h"

<<<<<<< HEAD
class Automate
{
private:
    CellHandler::CellHandler & m_cellHandler;
    Rule::Rule & m_rule;
    friend class AutomateHandler;
public:
    Automate(const CellHandler::CellHandler & cellHandler, const Rule::Rule & rule);
=======
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
>>>>>>> alex
    bool run(unsigned int nbSteps = 1);
};

#endif // AUTOMATE_H

#ifndef AUTOMATE_H
#define AUTOMATE_H

#include "cellhandler.h"
#include "rule.h"

class Automate
{
private:
    CellHandler::CellHandler & m_cellHandler;
    Rule::Rule & m_rule;
    friend class AutomateHandler;
public:
    Automate(const CellHandler::CellHandler & cellHandler, const Rule::Rule & rule);
    bool run(unsigned int nbSteps = 1);
};

#endif // AUTOMATE_H

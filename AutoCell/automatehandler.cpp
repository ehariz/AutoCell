#include "automatehandler.h"


AutomateHandler * m_activeAutomate = nullptr;

AutomateHandler::~AutomateHandler()
{

}

Automate::Automate & AutomateHandler::getActiveAutomate()
{
    if(!m_activeAutomate)
        m_activeAutomate = new Automate(???);
    return *m_activeAutomate;
}

void AutomateHandler::deleteActiveAutomate()
{
    if(m_activeAutomate)
        delete m_activeAutomate;
    m_activeAutomate = nullptr;
}

void AutomateHandler::setActiveAutomate(unsigned int activeAutomate)
{

}

#include "automatehandler.h"

/** \brief Initialization of the static value
*/
AutomateHandler * AutomateHandler::m_activeAutomateHandler = nullptr;

AutomateHandler::AutomateHandler()
{

}

AutomateHandler::~AutomateHandler()
{
    while(!m_ActiveAutomates.empty())
        delete(m_ActiveAutomates.first());
}

/** \brief Get the unique running automate handler instance or create one if there is no instance running
 *
 * \return the unique running automate handler instance
 */
AutomateHandler & AutomateHandler::getAutomateHandler()
{
    if (!m_activeAutomateHandler)
        m_activeAutomateHandler = new AutomateHandler;
    return *m_activeAutomateHandler;
}


/** \brief Delete the unique running automate instance if it exists
 */
void AutomateHandler::deleteAutomateHandler()
{
    if(m_activeAutomateHandler)
    {
        delete m_activeAutomateHandler;
        m_activeAutomateHandler = nullptr;
    }
}

/** \brief Set the active automate
 */

Automate * AutomateHandler::getAutomate(unsigned int indexAutomate){ //easier than throw catch
    if(indexAutomate > m_ActiveAutomates.size())
        return nullptr;
    return m_ActiveAutomates.at(indexAutomate);
}

unsigned int AutomateHandler::getNumberAutomates()const
{
    return m_ActiveAutomates.size();
}

void AutomateHandler::addAutomate(Automate * automate)
{
    m_ActiveAutomates.append(automate);
}


void AutomateHandler::deleteAutomate(Automate * automate)
{
    if(m_ActiveAutomates.contains(automate))
    {
        delete automate;
        m_ActiveAutomates.removeOne(automate);
    }
}


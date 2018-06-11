#include "automatehandler.h"

/** \brief Initialization of the static value
*/
AutomateHandler * AutomateHandler::m_activeAutomateHandler = nullptr;


/** \brief Construct an automate handler
 */
AutomateHandler::AutomateHandler()
{

}


/** \brief Delete all the automates contained in the automate handler
 */
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


/** \brief Delete the unique automate handler if it exists
 */
void AutomateHandler::deleteAutomateHandler()
{
    if(m_activeAutomateHandler)
    {
        delete m_activeAutomateHandler;
        m_activeAutomateHandler = nullptr;
    }
}


/** \brief Get an automate from the list according to its index
 *
 * \param indexAutomate Index of a specific automate in the automate list
 * \return Pointer on the requested automated if the parameter index fits with the list size
 *
 */
Automate * AutomateHandler::getAutomate(unsigned int indexAutomate){
    if(indexAutomate > m_ActiveAutomates.size())
        return nullptr;
    return m_ActiveAutomates.at(indexAutomate);
}


/** \brief Get the number of automates contained in the automate list
 *
 * \return number of automates in the automate list
 *
 */
unsigned int AutomateHandler::getNumberAutomates()const
{
    return m_ActiveAutomates.size();
}


/** \brief Add an automate in the automate list
 *
 * \param automate to be added to the automate list
 *
 */
void AutomateHandler::addAutomate(Automate * automate)
{
    m_ActiveAutomates.append(automate);
}


/** \brief Delete an automate from the automate list
 *
 * \param automate automate to delete
 *
 */
void AutomateHandler::deleteAutomate(Automate * automate)
{
    if(m_ActiveAutomates.contains(automate))
    {
        delete automate;
        m_ActiveAutomates.removeOne(automate);
    }
}

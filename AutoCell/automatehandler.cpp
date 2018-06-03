#include "automatehandler.h"

/** \brief Initialization of the static value
AutomateHandler * m_activeAutomate = nullptr;
 */

AutomateHandler::~AutomateHandler()
{

}

/** \brief Get the unique running automate instance or create one if there is no instance running
 *
 * \return the unique running automate instance
 */
Automate::Automate & AutomateHandler::getActiveAutomate()
{
    if(!m_activeAutomate)
        //m_activeAutomate = new Automate(???);
    return *m_activeAutomate;
}

/** \brief Delete the unique running automate instance if it exists
 */
void AutomateHandler::deleteActiveAutomate()
{
    if(m_activeAutomate)
        delete m_activeAutomate;
    m_activeAutomate = nullptr;
}

void AutomateHandler::setActiveAutomate(unsigned int activeAutomate)
{

}

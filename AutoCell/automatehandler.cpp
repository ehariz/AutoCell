#include "automatehandler.h"

/** \brief Initialization of the static value
<<<<<<< HEAD
AutomateHandler * m_activeAutomate = nullptr;
 */
=======
*/
AutomateHandler * m_activeAutomate = nullptr;
>>>>>>> alex

AutomateHandler::~AutomateHandler()
{

}

/** \brief Get the unique running automate instance or create one if there is no instance running
 *
 * \return the unique running automate instance
 */
<<<<<<< HEAD
Automate::Automate & AutomateHandler::getActiveAutomate()
{
    if(!m_activeAutomate)
        //m_activeAutomate = new Automate(???);
    return *m_activeAutomate;
}

=======
Automate & AutomateHandler::getActiveAutomate()
{
   /* if(!m_activeAutomate)
        m_activeAutomate = new Automate();
    return *m_activeAutomate;*/
}


>>>>>>> alex
/** \brief Delete the unique running automate instance if it exists
 */
void AutomateHandler::deleteActiveAutomate()
{
<<<<<<< HEAD
    if(m_activeAutomate)
        delete m_activeAutomate;
    m_activeAutomate = nullptr;
}

=======
    /*if(m_activeAutomate)
        delete m_activeAutomate;
    m_activeAutomate = nullptr;*/
}

/** \brief Set the active automate
 */
>>>>>>> alex
void AutomateHandler::setActiveAutomate(unsigned int activeAutomate)
{

}

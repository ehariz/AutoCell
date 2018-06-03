#ifndef AUTOMATEHANDLER_H
#define AUTOMATEHANDLER_H

#include "automate.h"

<<<<<<< HEAD
class AutomateHandler //Implementation of singleton design pattern
{
private:
    static AutomateHandler * m_activeAutomate;
=======
/** \class AutomateHandler
 * \brief Implementation of singleton design pattern
 */
class AutomateHandler
{
private:
    static AutomateHandler * m_activeAutomate; ///< active automate if existing, nullptr else
>>>>>>> alex
    AutomateHandler(const AutomateHandler & a) = delete;
    AutomateHandler & operator=(const AutomateHandler & a) = delete;
    ~AutomateHandler();
public:
    static Automate & getActiveAutomate();
    static void deleteActiveAutomate();
    void setActiveAutomate(unsigned int activeAutomate);
};

#endif // AUTOMATEHANDLER_H

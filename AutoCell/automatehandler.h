#ifndef AUTOMATEHANDLER_H
#define AUTOMATEHANDLER_H

#include "automate.h"


/** \class AutomateHandler
 * \brief Implementation of singleton design pattern to manage the Automates
 */
class AutomateHandler
{
private:
    QList<Automate*> m_ActiveAutomates; ///< list of existing automates
    static AutomateHandler * m_activeAutomateHandler; ///< active automate handler if existing, nullptr else

    AutomateHandler();
    AutomateHandler(const AutomateHandler & a) = delete;
    AutomateHandler & operator=(const AutomateHandler & a) = delete;
    ~AutomateHandler();

public:
    static AutomateHandler & getAutomateHandler();
    static void deleteAutomateHandler();

    Automate * getAutomate(int indexAutomate);
    unsigned int getNumberAutomates()const;

    void addAutomate(Automate * automate);
    void deleteAutomate(Automate * automate);
};


#endif // AUTOMATEHANDLER_H

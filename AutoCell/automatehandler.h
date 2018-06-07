#ifndef AUTOMATEHANDLER_H
#define AUTOMATEHANDLER_H

#include "automate.h"


/** \class AutomateHandler
 * \brief Implementation of singleton design pattern
 */
class AutomateHandler
{
private:
    QList<Automate*> m_ActiveAutomates; //better than QVector if we want to delete in the middle
    static AutomateHandler * m_activeAutomateHandler; ///< active automate if existing, nullptr else

    AutomateHandler();
    AutomateHandler(const AutomateHandler & a) = delete;
    AutomateHandler & operator=(const AutomateHandler & a) = delete;
    ~AutomateHandler();
public:
    static AutomateHandler & getAutomateHandler();
    static void deleteAutomateHandler();

    Automate * getAutomate(unsigned int indexAutomate);
    unsigned int getNumberAutomates()const;

    void addAutomate(Automate * automate);
    void deleteAutomate(Automate * automate);
};


#endif // AUTOMATEHANDLER_H

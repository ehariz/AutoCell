#ifndef AUTOMATEHANDLER_H
#define AUTOMATEHANDLER_H

#include "automate.h"

class AutomateHandler //Implementation of singleton design pattern
{
private:
    static AutomateHandler * m_activeAutomate;
    AutomateHandler(const AutomateHandler & a) = delete;
    AutomateHandler & operator=(const AutomateHandler & a) = delete;
    ~AutomateManager();
public:
    static Automate & getActiveAutomate();
    static void deleteActiveAutomate();
    void setActiveAutomate(unsigned int activeAutomate);
};

#endif // AUTOMATEHANDLER_H

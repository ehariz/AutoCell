#ifndef AUTOMATE_H
#define AUTOMATE_H
#include <QVector>

#include "cellhandler.h"
#include "rule.h"
#include "neighbourrule.h"
#include "matrixrule.h"


/** \class Automate
 * \brief
 */
class Automate
{
private:
    CellHandler* m_cellHandler; ///< CellHandler to go through
    QVector<const Rule*> m_rules; ///< Rules to use on the cells
    friend class AutomateHandler;

    bool loadRules(const QJsonObject &json);
public:
    Automate(QString filename);
    virtual ~Automate();


public:
    bool run(unsigned int nbSteps = 1);
    const CellHandler& getCellHandler() const;
};

#endif // AUTOMATE_H

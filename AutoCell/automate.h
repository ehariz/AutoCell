#ifndef AUTOMATE_H
#define AUTOMATE_H
#include <QVector>
#include <QList>

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
    QList<const Rule*> m_rules; ///< Rules to use on the cells
    friend class AutomateHandler;

    bool loadRules(const QJsonArray &json);
public:
    Automate(QString filename);
    Automate(const QVector<unsigned int> dimensions, CellHandler::generationTypes type = CellHandler::empty, unsigned int stateMax = 1, unsigned int density = 20);
    Automate(QString cellHandlerFilename, QString ruleFilename);
    virtual ~Automate();

    void addRule(const Rule* newRule);


public:
    bool run(unsigned int nbSteps = 1);
    const CellHandler& getCellHandler() const;
};

#endif // AUTOMATE_H

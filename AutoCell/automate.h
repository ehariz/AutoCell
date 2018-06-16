#ifndef AUTOMATE_H
#define AUTOMATE_H
#include <QVector>
#include <QList>

#include "cellhandler.h"
#include "rule.h"
#include "neighbourrule.h"
#include "matrixrule.h"


/** \class Automate
 * \brief Manage the application of rules on the cells
 */
class Automate
{
private:
    CellHandler* m_cellHandler = nullptr; ///< CellHandler to go through
    QList<const Rule*> m_rules; ///< Rules to use on the cells
    friend class AutomateHandler;

    bool loadRules(const QJsonArray &json);
public:
    Automate(QString filename);
    Automate(const QVector<unsigned int> dimensions, CellHandler::generationTypes type = CellHandler::empty, unsigned int stateMax = 1, unsigned int density = 20);
    Automate(QString cellHandlerFilename, QString ruleFilename);
    virtual ~Automate();

    bool saveRules(QString filename) const ;
    bool saveCells(QString filename) const ;
    bool saveAll(QString cellHandlerFilename, QString rulesFilename)const ;

    void addRuleFile(QString filename);
    void addRule(const Rule* newRule);
    void setRulePriority(const Rule* rule, unsigned int newPlace);
    const QList<const Rule *> &getRules() const;



public:
    bool run(unsigned int nbSteps = 1);
    const CellHandler& getCellHandler() const;
};

QList<const Rule*> generate1DRules(unsigned int automatonNumber);
const MatrixRule *getRuleFromNumber(int previousConfiguration, int nextState);

#endif // AUTOMATE_H

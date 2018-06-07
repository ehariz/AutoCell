#include "automate.h"

/** \brief Load the rules of the json given
 * \return Return false if something went wrong
 * \param json JsonObject wich contains the rules
 */
bool Automate::loadRules(const QJsonArray &json)
{

    for (QJsonArray::const_iterator it = json.begin(); it != json.end(); ++it)
    {
        if (!it->isObject())
            return false;
        QJsonObject ruleJson = it->toObject();

        if (!ruleJson.contains("type") || !ruleJson["type"].isString())
            return false;
        if (!ruleJson.contains("finalState") || !ruleJson["finalState"].isDouble())
            return false;
        if (!ruleJson.contains("currentStates") || !ruleJson["currentStates"].isArray())
            return false;

        QVector<unsigned int> currentStates;
        QJsonArray statesJson = ruleJson["currentStates"].toArray();
        for (unsigned int i = 0; i < statesJson.size(); i++)
        {
            if (!statesJson.at(i).isDouble())
                return false;
            currentStates.push_back(statesJson.at(i).toInt());
        }

        if (!ruleJson["type"].toString().compare("neighbour", Qt::CaseInsensitive))
        {
            if (!ruleJson.contains("neighbourNumberMin") || !ruleJson["neighbourNumberMin"].isDouble())
                return false;
            if (!ruleJson.contains("neighbourNumberMax") || !ruleJson["neighbourNumberMax"].isDouble())
                return false;



            QPair<unsigned int, unsigned int> nbrNeighbourInterval(ruleJson["neighbourNumberMin"].toInt(), ruleJson["neighbourNumberMax"].toInt());
            NeighbourRule *newRule;
            if (ruleJson.contains("neighbourStates"))
            {
                if (!ruleJson["neighbourStates"].isArray())
                    return false;
                QSet<unsigned int> neighbourStates;

                QJsonArray statesJson = ruleJson["neighbourStates"].toArray();
                for (unsigned int i = 0; i < statesJson.size(); i++)
                {
                    if (!statesJson.at(i).isDouble())
                        return false;
                    neighbourStates.insert(statesJson.at(i).toInt());
                }
                newRule = new NeighbourRule((unsigned int)ruleJson["finalState"].toInt(), currentStates, nbrNeighbourInterval, neighbourStates);
            }
            else
                newRule = new NeighbourRule((unsigned int)ruleJson["finalState"].toInt(), currentStates, nbrNeighbourInterval);
            m_rules.push_back(newRule);
        }
        else if (!ruleJson["type"].toString().compare("matrix", Qt::CaseInsensitive))
        {
            MatrixRule *newRule = new MatrixRule((unsigned int)ruleJson["finalState"].toInt(), currentStates);
            if (ruleJson.contains("neighbours"))
            {
                if (!ruleJson["neighbours"].isArray())
                    return false;
                QJsonArray neighboursJson = ruleJson["neighbours"].toArray();
                for (unsigned int i = 0; i < neighboursJson.size(); i++)
                {
                    if (!neighboursJson.at(i).isObject())
                        return false;

                    if (!neighboursJson.at(i).toObject().contains("relativePosition") || !neighboursJson.at(i).toObject()["relativePosition"].isArray())
                        return false;
                    if (!neighboursJson.at(i).toObject().contains("neighbourStates") || !neighboursJson.at(i).toObject()["neighbourStates"].isArray())
                        return false;

                    QVector<unsigned int> neighbourStates;


                    QJsonArray statesJson = neighboursJson.at(i).toObject()["neighbourStates"].toArray();
                    for (unsigned int j = 0; j < statesJson.size(); j++)
                    {
                        if (!statesJson.at(j).isDouble())
                            return false;
                        neighbourStates.push_back(statesJson.at(j).toInt());
                    }

                    QVector<short> relativePosition;
                    QJsonArray positionJson = neighboursJson.at(i).toObject()["relativePosition"].toArray();
                    for (unsigned int j = 0; j < positionJson.size(); j++)
                    {
                        if (!positionJson.at(j).isDouble())
                            return false;
                        relativePosition.push_back(positionJson.at(j).toInt());
                    }
                    if (relativePosition.size() != m_cellHandler->getDimensions().size())
                        return false;
                    newRule->addNeighbourState(relativePosition, neighbourStates);
                }

            }
            m_rules.push_back(newRule);


        }
        else
            return false;

    }
    return true;
}

/** \brief Create an automate with only a cellHandler from file
 *
 * \param cellHandlerFilename File to load
 */
Automate::Automate(QString cellHandlerFilename)
{
    m_cellHandler = new CellHandler(cellHandlerFilename);

}

/** \brief Create an automate with only a cellHandler with parameters
 *
 * \param dimensions Dimensions of the CellHandler
 * \param type Generation type, empty by default
 * \param stateMax Generate states between 0 and stateMax
 * \param density Average (%) of non-zeros
 */
Automate::Automate(const QVector<unsigned int> dimensions, CellHandler::generationTypes type, unsigned int stateMax, unsigned int density)
{
    m_cellHandler = new CellHandler(dimensions, type, stateMax, density);

}

/** \brief Create an automate from files
 *
 * \param cellHandlerFilename File of the cellHandler
 * \param ruleFilename File of the rules
 */
Automate::Automate(QString cellHandlerFilename, QString ruleFilename)
{
    m_cellHandler = new CellHandler(cellHandlerFilename);

    QFile ruleFile(ruleFilename);
    if (!ruleFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning("Couldn't open given file.");
        throw QString(QObject::tr("Couldn't open given file"));
    }

    QJsonParseError parseErr;
    QJsonDocument loadDoc(QJsonDocument::fromJson(ruleFile.readAll(), &parseErr));

    ruleFile.close();


    if (loadDoc.isNull() || loadDoc.isEmpty())
    {
        qWarning() << "Could not read data : ";
        qWarning() << parseErr.errorString();
        throw QString(parseErr.errorString());
    }

    if (!loadDoc.isArray())
    {
        qWarning() << "We need an array of rules !";
        throw QString(QObject::tr("We need an array of rules!"));
    }

    loadRules(loadDoc.array());

}

/** \brief Destructor : free the CellHandler and the rules !
 */
Automate::~Automate()
{
    delete m_cellHandler;
    for (QList<const Rule*>::iterator it = m_rules.begin(); it != m_rules.end(); ++it)
    {
        delete *it;
    }
}

/** \brief Save automate's rules in the file
 * \return False if something went wrong
 */
bool Automate::saveRules(QString filename) const
{
    QFile ruleFile(filename);
    if (!ruleFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning("Couldn't open given file.");
        throw QString(QObject::tr("Couldn't open given file"));
    }

    QJsonArray array;

    for (QList<const Rule*>::const_iterator it = m_rules.cbegin(); it != m_rules.cend(); ++it)
        array.append((*it)->toJson());

    QJsonDocument doc(array);

    ruleFile.write(doc.toJson());

    return true;
}

/** \brief Save cellHandler
 */
bool Automate::saveCells(QString filename) const
{
    if (m_cellHandler != nullptr)
        return m_cellHandler->save(filename);
    return false;
}

/** \brief Save both rules and cellHandler in the differents files
 */
bool Automate::saveAll(QString cellHandlerFilename, QString rulesFilename) const
{
    return saveRules(rulesFilename) && saveCells(cellHandlerFilename);
}

/** \brief Add a new rule to the Automate. Careful, the rule will be destroyed with the Automate
 */
void Automate::addRule(const Rule *newRule)
{
    m_rules.push_back(newRule);
}

/** \brief Modify the place of the rule in the priority list.
 *
 * 2 rules can't have the same priority rank
 * \param rule Rule to move
 * \param newPlace New place of the rule
 */
void Automate::setRulePriority(const Rule *rule, unsigned int newPlace)
{
    m_rules.move(m_rules.indexOf(rule), newPlace);
}

/** \brief Return all the rules
 */
const QList<const Rule *> &Automate::getRules() const
{
   return m_rules;
}

/** \brief Apply the rule on the cells grid nbSteps times
 *
 * \param nbSteps number of iterations of the automate on the cell grid
 */
bool Automate::run(unsigned int nbSteps) //void instead ?
{
    for(unsigned int i = 0; i<nbSteps; ++i)
    {
        for (CellHandler::iterator it = m_cellHandler->begin(); it != m_cellHandler->end(); ++it)
        {
            for (QList<const Rule*>::iterator rule = m_rules.begin(); rule != m_rules.end() ; ++rule)
            {
                if((*rule)->matchCell(*it)) //if the cell matches with the rule, its state is changed
                {
                    it->setState((*rule)->getCellOutputState());
                    break;
                }
            }


        }
        m_cellHandler->nextStates(); //apply the changes to all the cells simultaneously
    }
    return true;

}

/** \brief Accessor of m_cellHandler
 */
const CellHandler &Automate::getCellHandler() const
{
    return *m_cellHandler;
}

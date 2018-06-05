#include "automate.h"

/** \brief Load the rules of the json given
 * \return Return false if something went wrong
 * \param json JsonObject wich contains the rules
 */
bool Automate::loadRules(const QJsonObject &json)
{
    if (!json.contains("rules") || !json["rules"].isArray())
        return false;
    QJsonArray ruleList = json["rules"].toArray();

    for (QJsonArray::iterator it = ruleList.begin(); it != ruleList.end(); ++it)
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

/** \brief Automate constructor from file
 *
 * \todo Use 2 files for the departState and the rules
 *
 * \param filename File to load
 */
Automate::Automate(QString filename)
{
    QFile loadFile(filename);
    if (!loadFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning("Couldn't open given file.");
        throw QString(QObject::tr("Couldn't open given file"));
    }

    QJsonParseError parseErr;
    QJsonDocument loadDoc(QJsonDocument::fromJson(loadFile.readAll(), &parseErr));



    if (loadDoc.isNull() || loadDoc.isEmpty()) {
        qWarning() << "Could not read data : ";
        qWarning() << parseErr.errorString();
        throw QString(parseErr.errorString());
    }

    m_cellHandler = new CellHandler(filename);

    if (!loadRules(loadDoc.object()))
    {
        qWarning("File not valid");
        throw QString(QObject::tr("File not valid"));
    }

}

/** \brief Destructor : free the CellHandler and the rules !
 */
Automate::~Automate()
{
    delete m_cellHandler;
    for (QVector<const Rule*>::iterator it = m_rules.begin(); it != m_rules.end(); ++it)
    {
        delete *it;
    }
}

/** \brief Apply the rule on the cells grid nbSteps times
 *
 * \param nbSteps number of iterations of the automate on the cell grid
 * \param neighbour New neighbour
 * \return False if the neighbour already exists
 */
bool Automate::run(unsigned int nbSteps) //void instead ?
{
    for(unsigned int i = 0; i<nbSteps; ++i)
    {
        for (CellHandler::iterator it = m_cellHandler->begin(); it != m_cellHandler->end(); ++it)
        {
            for (QVector<const Rule*>::iterator rule = m_rules.begin(); rule != m_rules.end() ; ++rule)
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

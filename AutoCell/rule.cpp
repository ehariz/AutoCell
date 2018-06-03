#include "rule.h"

<<<<<<< HEAD
/** \brief Checks if the number of neighbours matching the state condition belongs to the condition interval
 *
 * \param number of neighbours matching the rule condition regarding their values
 * \return True if the number of neighbours matches with the interval condition
 */
bool Rule::inInterval(unsigned int matchingNeighbours)const
{
    if(i >= m_neighbourInterval.first && i<=m_neighbourInterval.second)
        return true;
    else
        return false;
}

/** \brief Constructs a rule with the parameters.
 *
 *
 * \param current cell
 */
Rule::Rule(QVector<unsigned int> currentCellValues, QPair<unsigned int, unsigned int> intervalNumber, QSet<unsigned int> intervalValues, unsigned int outputState):
        m_currentCellPossibleValues(currentCellValues), m_neighbourInterval(intervalNumber), m_neighbourPossibleValues(intervalValues), m_cellOutputState(outputState)
=======
Rule::Rule(QVector<unsigned int> currentCellValues, unsigned int outputState):
        m_currentCellPossibleValues(currentCellValues), m_cellOutputState(outputState)
>>>>>>> alex
{

}

<<<<<<< HEAD
Rule::~Rule()
{

}

/** \brief Checks if the input cell satisfies the rule condition.
 *
 *
 * \param current cell
 * \return True if the cell matches the rule condition
 */
bool Rule::matchCell(const Cell & c)const
{
    unsigned int matchingNeighbours = 0;
   // QSet<unsigned int> set;
    //QSet<unsigned int> m_neighbourPossibleValues;
    //set<<3<<2<<5<<9;
    QSet<unsigned int>::const_iterator i = m_neighbourPossibleValues.constBegin();
    while (i != m_neighbourPossibleValues.constEnd()) {
        //std::cout<<*i;
        matchingNeighbours += c.countNeighbours(*i);
        ++i;
    }
    if(m_currentCellPossibleValues.contains(c.getState()) && inInterval(matchingNeighbours))
        return true; //the rule is applied to the cell
    else
        return false; //the rule is not applied to the cell
}

/** \brief Get the rule output state that will be the next state if the cell matches the rule condition.
=======
/** \brief Get the rule output state that will be the next state if the cell matches the rule condition
>>>>>>> alex
 */
unsigned int Rule::getCellOutputState()const
{
        return m_cellOutputState;
}
<<<<<<< HEAD

//ATTENTION COPY & PASTE FROM CELLHANDLER.H
/** \brief Save the CellHandler current configuration in the file given
 *
 * \param filename Path to the file
 * \return False if there was a problem
 *
 * \throw QString Impossible to open the file
 */
/*
bool CellHandler::save(QString filename) const
{
    QFile saveFile(filename);
    if (!saveFile.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't create or open given file.");
        throw QString(QObject::tr("Couldn't create or open given file"));
    }

    QJsonObject json;
    QString stringDimension;
    // Creation of the dimension string
    for (int i = 0; i < m_dimensions.size(); i++)
    {
        if (i != 0)
            stringDimension.push_back("x");
        stringDimension.push_back(QString::number(m_dimensions.at(i)));
    }
    json["dimensions"] = QJsonValue(stringDimension);

    QJsonArray cells;
    for (CellHandler::const_iterator it = begin(); it != end(); ++it)
    {
        cells.append(QJsonValue((int)it->getState()));
    }
    json["cells"] = cells;


    QJsonDocument saveDoc(json);
    saveFile.write(saveDoc.toJson());

    saveFile.close();
    return true;
}
*/
=======
>>>>>>> alex

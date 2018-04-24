#include <iostream>
#include "cellhandler.h"


/** \fn CellHandler::CellHandler(QString filename)
 * \brief Construct all the cells from the json file given
 *
 * The size of "cells" array must be the product of all dimensions (60 in the following example).
 * Typical Json file:
 * \code
 * {
 * "dimensions":"3x4x5",
 * "cells":[0,1,4,4,2,5,3,4,2,4,
 *          4,2,5,0,0,0,0,0,0,0,
 *          2,4,1,1,1,1,1,2,1,1,
 *          0,0,0,0,0,0,2,2,2,2,
 *          3,4,5,1,2,0,9,0,0,0,
 *          1,2,0,0,0,0,1,2,3,2]
 * }
 * \endcode
 *
 * \param filename Json file which contains the description of all the cells
 * \throw QString Unreadable file
 * \throw QString Empty file
 * \throw QString Not valid file
 */
CellHandler::CellHandler(const QString filename)
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
    }

    // Loadding of the json file
    if (!load(loadDoc.object()))
    {
        qWarning("File not valid");
        throw QString(QObject::tr("File not valid"));
    }

    foundNeighbours();


}

/** \fn CellHandler::CellHandler(const QVector<unsigned int> dimensions, generationTypes type, unsigned int stateMax, unsigned int density)
 * \brief Construct a CellHandler of the given dimension
 *
 * If generationTypes is given, the CellHandler won't be empty.
 *
 * \param dimensions Dimensions of the CellHandler
 * \param type Generation type, empty by default
 * \param stateMax Generate states between 0 and stateMax
 * \param density Average (%) of non-zeros
 */
CellHandler::CellHandler(const QVector<unsigned int> dimensions, generationTypes type, unsigned int stateMax, unsigned int density)
{
    m_dimensions = dimensions;
    QVector<unsigned int> position;
    unsigned int size = 1;

    // Set position vector to 0

    for (unsigned short i = 0; i < m_dimensions.size(); i++)
    {
        position.push_back(0);
        size *= m_dimensions.at(i);
    }


    // Creation of cells
    for (unsigned int j = 0; j < size; j++)
    {
        m_cells.insert(position, new Cell(0));

        positionIncrement(position);
    }

    if (type != empty)
        generate(type, stateMax, density);

}

/** \fn CellHandler::~CellHandler()
 * \brief Destroys all cells in the CellHandler
 */
CellHandler::~CellHandler()
{
    for (QMap<QVector<unsigned int>, Cell* >::iterator it = m_cells.begin(); it != m_cells.end(); ++it)
    {
        delete it.value();
    }
}

/** \fn Cell *CellHandler::getCell(const QVector<unsigned int> position) const
 * \brief Access the cell to the given position
 */
Cell *CellHandler::getCell(const QVector<unsigned int> position) const
{
    return m_cells.value(position);
}

/** \fn QVector<unsigned int> CellHandler::getDimensions()
 * \brief Accessor of m_dimensions
 */
QVector<unsigned int> CellHandler::getDimensions()
{
    return m_dimensions;
}

/** \fn void CellHandler::nextStates()
 * \brief Valid the state of all cells
 *
 */
void CellHandler::nextStates()
{
    for (QMap<QVector<unsigned int>, Cell* >::iterator it = m_cells.begin(); it != m_cells.end(); ++it)
    {
        it.value()->validState();
    }
}

/** \fn bool CellHandler::save(QString filename)
 * \brief Save the CellHandler current configuration in the file given
 *
 * \param filename Path to the file
 * \return False if there was a problem
 *
 * \throw QString Impossible to open the file
 */
bool CellHandler::save(QString filename)
{
    QFile saveFile(filename);
    if (!saveFile.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't create or open given file.");
        throw QString(QObject::tr("Couldn't create or open given file"));
    }

    QJsonObject json;
    QString stringDimension;
    // Creation of the dimension string
    for (unsigned int i = 0; i < m_dimensions.size(); i++)
    {
        if (i != 0)
            stringDimension.push_back("x");
        stringDimension.push_back(QString::number(m_dimensions.at(i)));
    }
    json["dimensions"] = QJsonValue(stringDimension);

    QJsonArray cells;
    for (CellHandler::iterator it = begin(); it != end(); ++it)
    {
        cells.append(QJsonValue((int)it->getState()));
    }
    json["cells"] = cells;


    QJsonDocument saveDoc(json);
    saveFile.write(saveDoc.toJson());

    saveFile.close();
    return true;
}

/** \fn void CellHandler::generate(CellHandler::generationTypes type, unsigned int stateMax, unsigned short density)
 * \brief Replace Cell values by random values (symetric or not)
 *
 * \param type Type of random generation
 * \param stateMax Generate states between 0 and stateMax
 * \param density Average (%) of non-zeros
 */
void CellHandler::generate(CellHandler::generationTypes type, unsigned int stateMax, unsigned short density)
{
    if (type == random)
    {
        QVector<unsigned int> position;
        for (unsigned short i = 0; i < m_dimensions.size(); i++)
        {
            position.push_back(0);
        }
        QRandomGenerator generator((float)qrand()*(float)time_t()/RAND_MAX);
        for (unsigned int j = 0; j < m_cells.size(); j++)
        {
            unsigned int state = 0;
            // 0 have (1-density)% of chance of being generate
            if (generator.generateDouble()*100.0 < density)
                 state = (float)(generator.generateDouble()*stateMax) +1;
            if (state > stateMax)
                state = stateMax;
            m_cells.value(position)->forceState(state);

            positionIncrement(position);
        }
    }
    else if (type == symetric)
    {
        QVector<unsigned int> position;
        for (unsigned short i = 0; i < m_dimensions.size(); i++)
        {
            position.push_back(0);
        }

        QRandomGenerator generator((float)qrand()*(float)time_t()/RAND_MAX);
        QVector<unsigned int> savedStates;
        for (unsigned int j = 0; j < m_cells.size(); j++)
        {
            if (j % m_dimensions.at(0) == 0)
                savedStates.clear();
            if (j % m_dimensions.at(0) < (m_dimensions.at(0)+1) / 2)
            {
                unsigned int state = 0;
                // 0 have (1-density)% of chance of being generate
                if (generator.generateDouble()*100.0 < density)
                    state = (float)(generator.generateDouble()*stateMax) +1;
                if (state > stateMax)
                    state = stateMax;
                savedStates.push_back(state);
                m_cells.value(position)->forceState(state);
            }
            else
            {
                unsigned int i = savedStates.size() - (j % m_dimensions.at(0) - (m_dimensions.at(0)-1)/2 + (m_dimensions.at(0) % 2 == 0 ? 0 : 1));
                m_cells.value(position)->forceState(savedStates.at(i));
            }
            positionIncrement(position);


        }

    }
}

/** \fn void CellHandler::print(std::ostream &stream)
 * \brief Print in the given stream the CellHandler
 *
 * \param stream Stream to print into
 */
void CellHandler::print(std::ostream &stream)
{
    for (iterator it = begin(); it != end(); ++it)
    {
        for (unsigned int d = 0; d < it.changedDimension(); d++)
            stream << std::endl;
        stream << it->getState() << " ";
    }

}

/** \fn CellHandler::iterator CellHandler::begin()
 * \brief Give the iterator which corresponds to the current CellHandler
 */
CellHandler::iterator CellHandler::begin()
{
    return iterator(this);
}

/** \fn bool CellHandler::end()
 * \brief End condition of the iterator
 *
 * See iterator::operator!=(bool finished) for further information.
 */
bool CellHandler::end()
{
    return true;
}

/** \fn bool CellHandler::load(const QJsonObject &json)
 * \brief Load the config file in the CellHandler
 *
 * Exemple of a way to print cell states :
 * \code
 * QVector<unsigned int> position;
 * for (unsigned short i = 0; i < m_dimensions.size(); i++)
 * {
 *      position.push_back(0);
 * }
 * for (unsigned int j = 0; j < m_cells.size(); j++)
 * {
 *      std::cout << m_cells.value(position)->getState() << " ";
 *      position.replace(0, position.at(0)+1);
 *      for (unsigned short i = 0; i < m_dimensions.size(); i++)
 *      {
 *          if (position.at(i) >= m_dimensions.at(i))
 *          {
 *              position.replace(i, 0);
 *              std::cout << std::endl;
 *              if (i + 1 != m_dimensions.size())
 *                  position.replace(i+1, position.at(i+1)+1);
 *          }
 *
 *      }
 * }
 * \endcode
 *
 * \param json Json Object which contains the grid configuration
 * \return False if the Json Object is not correct
 */
bool CellHandler::load(const QJsonObject &json)
{
    if (!json.contains("dimensions") || !json["dimensions"].isString())
        return false;

    // RegExp to validate dimensions field format : "10x10"
    QRegExpValidator dimensionValidator(QRegExp("([0-9]*x?)*"));
    QString stringDimensions = json["dimensions"].toString();
    int pos= 0;
    if (dimensionValidator.validate(stringDimensions, pos) != QRegExpValidator::Acceptable)
        return false;

    // Split of dimensions field : "10x10" => "10", "10"
    QRegExp rx("x");
    QStringList list = json["dimensions"].toString().split(rx, QString::SkipEmptyParts);

    unsigned int product = 1;
    // Dimensions construction
    for (unsigned int i = 0; i < list.size(); i++)
    {
        product = product * list.at(i).toInt();
        m_dimensions.push_back(list.at(i).toInt());
    }
    if (!json.contains("cells") || !json["cells"].isArray())
        return false;

    QJsonArray cells = json["cells"].toArray();
    if (cells.size() != product)
        return false;

    QVector<unsigned int> position;
    // Set position vector to 0
    for (unsigned short i = 0; i < m_dimensions.size(); i++)
    {
        position.push_back(0);
    }

    // Creation of cells
    for (unsigned int j = 0; j < cells.size(); j++)
    {
        if (!cells.at(j).isDouble())
            return false;
        if (cells.at(j).toDouble() < 0)
            return false;
        m_cells.insert(position, new Cell(cells.at(j).toDouble()));

        positionIncrement(position);
    }

    return true;

}

/** \fn bool CellHandler::foundNeighbours()
 * \brief Set the neighbours of each cells
 *
 * Careful, this is in O(n*3^d), with n the number of cells and d the number of dimensions
 *
 */
void CellHandler::foundNeighbours()
{
    QVector<unsigned int> currentPosition;
    // Set position vector to 0
    for (unsigned short i = 0; i < m_dimensions.size(); i++)
    {
        currentPosition.push_back(0);
    }
    // Modification of all the cells
    for (unsigned int j = 0; j < m_cells.size(); j++)
    {
        // Get the list of the neighbours positions
        // This function is recursive
        QVector<QVector<unsigned int> > listPosition(getListNeighboursPositions(currentPosition));

        // Adding neighbours
        for (unsigned int i = 0; i < listPosition.size(); i++)
            m_cells.value(currentPosition)->addNeighbour(m_cells.value(listPosition.at(i)));

        positionIncrement(currentPosition);
    }
}

/** \fn void CellHandler::positionIncrement(QVector<unsigned int> &pos, unsigned int value) const
 * \brief Increment the QVector given by the value choosen
 *
 * Careful, when the position reach the maximum, it goes to zero without leaving the function
 *
 * \param pos Position to increment
 * \param value Value to add, 1 by default
 */
void CellHandler::positionIncrement(QVector<unsigned int> &pos, unsigned int value) const
{
    pos.replace(0, pos.at(0) + value); // adding the value to the first digit

    // Carry management
    for (unsigned short i = 0; i < m_dimensions.size(); i++)
    {
        if (pos.at(i) >= m_dimensions.at(i) && pos.at(i) < m_dimensions.at(i)*2)
        {
            pos.replace(i, 0);
            if (i + 1 != m_dimensions.size())
                pos.replace(i+1, pos.at(i+1)+1);
        }
        else if (pos.at(i) >= m_dimensions.at(i))
        {
            pos.replace(i, pos.at(i) - m_dimensions.at(i));
            if (i + 1 != m_dimensions.size())
                pos.replace(i+1, pos.at(i+1)+1);
            i--;
        }

    }
}

/** \fn QVector<QVector<unsigned int> >& CellHandler::getListNeighboursPositions(const QVector<unsigned int> position) const
 * \brief Prepare the call of the recursive version of itself
 *
 * \param position Position of the central cell (x1,x2,x3,..,xn)
 * \return List of positions
 */
QVector<QVector<unsigned int> >& CellHandler::getListNeighboursPositions(const QVector<unsigned int> position) const
{
    QVector<QVector<unsigned int> > *list = getListNeighboursPositionsRecursive(position, position.size(), position);
    // We remove the position of the cell
    list->removeAll(position);
    return *list;
}

/** \fn QVector<QVector<unsigned int> >* CellHandler::getListNeighboursPositionsRecursive(const QVector<unsigned int> position, unsigned int dimension, QVector<unsigned int> lastAdd) const
 * \brief Recursive function which browse the position possibilities tree
 *
 * Careful, the complexity is in O(3^dimension)<br>
 * Piece of the tree:
 * \code
 *             x_d -1
 *           /
 * x_(d-1)-1/_ x_d
 *          \
 *           \
 *             x_d +1
 *
 *             x_d -1
 *           /
 * x_(d-1)  /_ x_d
 *          \
 *           \
 *             x_d +1
 *
 *             x_d -1
 *           /
 * x_(d-1)+1/ x_d
 *          \
 *           \
 *             x_d +1
 * \endcode
 * The path in the tree to reach the leaf give the position
 *
 * \param position Position of the cell
 * \param dimension Current working dimension (number of the digit). Dimension = 2 <=> working on x2 coordinates on (x1, x2, x3, ..., xn) vector
 * \param lastAdd Last position added. Like the father node of the new tree
 * \return List of position
 */
QVector<QVector<unsigned int> >* CellHandler::getListNeighboursPositionsRecursive(const QVector<unsigned int> position, unsigned int dimension, QVector<unsigned int> lastAdd) const
{
    if (dimension == 0) // Stop condition
    {
        QVector<QVector<unsigned int> > *list = new QVector<QVector<unsigned int> >;
        return list;
    }
    QVector<QVector<unsigned int> > *listPositions = new QVector<QVector<unsigned int> >;

    QVector<unsigned int> modifiedPosition(lastAdd);

    // "x_d - 1" tree
    if (modifiedPosition.at(dimension-1) != 0) // Avoid "negative" position
        modifiedPosition.replace(dimension-1, position.at(dimension-1) - 1);
    listPositions->append(*getListNeighboursPositionsRecursive(position, dimension -1, modifiedPosition));
    if (!listPositions->count(modifiedPosition))
        listPositions->push_back(modifiedPosition);

    // "x_d" tree
    modifiedPosition.replace(dimension-1, position.at(dimension-1));
    listPositions->append(*getListNeighboursPositionsRecursive(position, dimension -1, modifiedPosition));
    if (!listPositions->count(modifiedPosition))
        listPositions->push_back(modifiedPosition);

    // "x_d + 1" tree
    if (modifiedPosition.at(dimension -1) + 1 < m_dimensions.at(dimension-1)) // Avoid position out of the cell space
        modifiedPosition.replace(dimension-1, position.at(dimension-1) +1);
    listPositions->append(*getListNeighboursPositionsRecursive(position, dimension -1, modifiedPosition));
    if (!listPositions->count(modifiedPosition))
        listPositions->push_back(modifiedPosition);

    return listPositions;

}

/** \fn CellHandler::iterator::iterator(const CellHandler *handler):
 * \brief Construct an initial iterator to browse the CellHandler
 *
 * \param handler CellHandler to browse
 */
CellHandler::iterator::iterator(const CellHandler *handler):
        m_handler(handler), m_changedDimension(0)
{
    // Initialisation of m_position
    for (unsigned short i = 0; i < handler->m_dimensions.size(); i++)
    {
        m_position.push_back(0);
    }
    m_zero = m_position;
}

/** \fn CellHandler::iterator &CellHandler::iterator::operator++()
 * \brief Increment the current position and handle dimension changes
 */
CellHandler::iterator &CellHandler::iterator::operator++()
{
    m_position.replace(0, m_position.at(0) + 1); // adding the value to the first digit

    m_changedDimension = 0;
    // Carry management
    for (unsigned short i = 0; i < m_handler->m_dimensions.size(); i++)
    {
        if (m_position.at(i) >= m_handler->m_dimensions.at(i))
        {
            m_position.replace(i, 0);
            m_changedDimension++;
            if (i + 1 != m_handler->m_dimensions.size())
                m_position.replace(i+1, m_position.at(i+1)+1);
        }

    }
    // If we return to zero, we have finished
    if (m_position == m_zero)
        m_finished = true;

    return *this;

}

/** \fn Cell *CellHandler::iterator::operator->()
 * \brief Get the current cell
 */
Cell *CellHandler::iterator::operator->() const
{
    return m_handler->m_cells.value(m_position);
}

/** \fn Cell *CellHandler::iterator::operator->()
 * \brief Get the current cell
 */
Cell *CellHandler::iterator::operator*() const
{
    return m_handler->m_cells.value(m_position);
}

/** \fn unsigned int CellHandler::iterator::changedDimension() const
 * \brief Return the number of dimensions we change
 *
 * For example, if we were at the (3,4,4) cell, and we incremented the position,
 * we are now at (4,0,0), and changedDimension return 2 (because of the 2 zeros).
 */
unsigned int CellHandler::iterator::changedDimension() const
{
    return m_changedDimension;
}


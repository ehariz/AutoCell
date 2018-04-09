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
 */
CellHandler::CellHandler(QString filename)
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

/** \fn bool CellHandler::load(const QJsonObject &json)
 * \brief Load the config file in the CellHandler
 *
 * Exemple of a way to print cell states :
 * \code
 * position.clear();
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
 *```
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
 *```
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

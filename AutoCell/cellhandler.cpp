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
/** \fn bool CellHandler::load(const QJsonObject &json)
 * \brief Load the config file in the CellHandler
 *
 * Exemple of a way to print cell states :
 * \code
 * position.clear();
 * for (unsigned short i = 0; i < m_dimensions.size(); i++)
 * {
 *  position.push_back(0);
 * }
 * for (unsigned int j = 0; j < m_cells.size(); j++)
 * {
 *  std::cout << m_cells.value(position)->getState() << " ";
 *  position.replace(0, position.at(0)+1);
 *  for (unsigned short i = 0; i < m_dimensions.size(); i++)
 *  {
 *      if (position.at(i) >= m_dimensions.at(i))
 *      {
 *          position.replace(i, 0);
 *          std::cout << std::endl;
 *          if (i + 1 != m_dimensions.size())
 *              position.replace(i+1, position.at(i+1)+1);
 *      }
 *
 *  }
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

        position.replace(0, position.at(0)+1);
        for (unsigned short i = 0; i < m_dimensions.size(); i++)
        {
            if (position.at(i) >= m_dimensions.at(i))
            {
                position.replace(i, 0);
                if (i + 1 != m_dimensions.size())
                    position.replace(i+1, position.at(i+1)+1);
            }

        }
    }


    return true;

}

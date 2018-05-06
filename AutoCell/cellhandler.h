#ifndef CELLHANDLER_H
#define CELLHANDLER_H

#include <QString>
#include <QFile>
#include <QJsonDocument>
#include <QtWidgets>
#include <QMap>
#include <QRegExpValidator>
#include <QDebug>

#include "cell.h"

/** \class CellHandler
 * \brief Cell container and cell generator
 *
 * Generate cells from a json file.
 */
class CellHandler
{
public:
    /** \class iterator
     * \brief Implementation of iterator design pattern
     *
     * Example of use:
     * \code
     * CellHandler handler("file.atc");
     * for (CellHandler::iterator it = handler.begin(); it != handler.end(); ++it)
     * {
     *      for (unsigned int i = 0; i < it.changedDimension(); i++)
     *          std::cout << std::endl;
     *      std::cout << it->getState() << " ";
     * }
     * \endcode
     * This code will print each cell states and go to a new line when there is a change of dimension.
     * So if there is 3 dimensions, there will be a empty line between 2D groups.
     */
    class iterator
    {
        friend class CellHandler;
    public:
        iterator(const CellHandler* handler);

        iterator& operator++();
        Cell* operator->() const;
        Cell* operator*() const;

        bool operator!=(bool finished) const { return (m_finished != finished); }
        unsigned int changedDimension() const;



    private:
        const CellHandler *m_handler; ///< CellHandler to go through
        QVector<unsigned int> m_position; ///< Current position of the iterator
        bool m_finished = false; ///< If we reach the last position
        QVector<unsigned int> m_zero; ///< Nul vector of the good dimension (depend of m_handler)
        unsigned int m_changedDimension; ///< Save the number of dimension change
    };

    /** \enum generationTypes
     * \brief Type of random generation
     */
    enum generationTypes {
        empty, ///< Only empty cells
        random, ///< Random cells
        symetric ///< Random cells but with vertical symetry (on the 1st dimension component)
    };

    CellHandler(const QString filename);
    CellHandler(const QVector<unsigned int> dimensions, generationTypes type = empty, unsigned int stateMax = 1, unsigned int density = 20);
    virtual ~CellHandler();

    Cell* getCell(const QVector<unsigned int> position) const;
    QVector<unsigned int> getDimensions() const;
    void nextStates() const;

    bool save(QString filename) const;

    void generate(generationTypes type, unsigned int stateMax = 1, unsigned short density = 50);
    void print(std::ostream &stream) const;

    iterator begin() const;
    bool end() const;

private:
    bool load(const QJsonObject &json);
    void foundNeighbours();
    void positionIncrement(QVector<unsigned int> &pos, unsigned int value = 1) const;
    QVector<QVector<unsigned int> > *getListNeighboursPositionsRecursive(const QVector<unsigned int> position, unsigned int dimension, QVector<unsigned int> lastAdd) const;
    QVector<QVector<unsigned int> > &getListNeighboursPositions(const QVector<unsigned int> position) const;

    QVector<unsigned int> m_dimensions; ///< Vector of x dimensions
    QMap<QVector<unsigned int>, Cell* > m_cells; ///< Map of cells, with a x dimensions vector as key
};

#endif // CELLHANDLER_H

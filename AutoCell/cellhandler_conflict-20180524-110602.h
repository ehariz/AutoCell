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



/** \brief Cell container and cell generator
 *
 * Generate cells from a json file.
 */
class CellHandler
{

    /** \brief Implementation of iterator design pattern with a template to generate iterator and const_iterator
     * at the same time.
     *
     *
     * Example of use:
     * \code
     * CellHandler handler("file.atc");
     * for (CellHandler::const_iterator it = handler.begin(); it != handler.end(); ++it)
     * {
     *      for (unsigned int i = 0; i < it.changedDimension(); i++)
     *          std::cout << std::endl;
     *      std::cout << it->getState() << " ";
     * }
     * \endcode
     * This code will print each cell states and go to a new line when there is a change of dimension.
     * So if there are 3 dimensions, there will be a empty line between 2D groups.
     */
    template <typename CellHandler_T, typename Cell_T, typename CellsIterator_T>
    class iteratorT
    {
        friend class CellHandler;
    public:
        iteratorT(CellHandler_T* handler);

        iteratorT& operator++();
        Cell_T* operator->() const;
        Cell_T* operator*() const;

        bool operator!=(bool finished) const { return (m_finished != finished); }
        unsigned int changedDimension() const;



    private:
        CellHandler_T *m_handler; ///< CellHandler to go through
        QVector<unsigned int> m_position; ///< Current position of the iterator
        CellsIterator_T m_it;
        bool m_finished = false; ///< If we reach the last position
        QVector<unsigned int> m_zero; ///< Nul vector of the good dimension (depend of m_handler)
        unsigned int m_changedDimension = 0; ///< Save the number of dimension change
    };
public:
    typedef iteratorT<const CellHandler, const Cell, QMap<QVector<unsigned int>, Cell* >::const_iterator> const_iterator;
    typedef iteratorT<CellHandler, Cell, QMap<QVector<unsigned int>, Cell* >::iterator> iterator;

    /** \brief Type of random generation
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
    int size() const;
    QVector<unsigned int> getDimensions() const;
    void nextStates() const;

    bool save(QString filename) const;

    void generate(generationTypes type, unsigned int stateMax = 1, unsigned short density = 50);
    void print(std::ostream &stream) const;

    const_iterator begin() const;
    iterator begin();
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

template class CellHandler::iteratorT<CellHandler, Cell, QMap<QVector<unsigned int>, Cell* >::iterator>;
template class CellHandler::iteratorT<const CellHandler, const Cell, QMap<QVector<unsigned int>, Cell* >::const_iterator>;

#endif // CELLHANDLER_H

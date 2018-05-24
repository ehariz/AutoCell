#ifndef CELL_H
#define CELL_H

#include <QVector>
#include <QDebug>

/** \class Cell
 * \brief Contains the state, the next state and the neighbours
 */
class Cell
{
public:
    Cell(unsigned int state = 0);

    void setState(unsigned int state);
    void validState();
    void forceState(unsigned int state);
    unsigned int getState() const;

    bool addNeighbour(const Cell* neighbour, const QVector<short> relativePosition);
    QMap<QVector<short>, const Cell*> getNeighbours() const;
    const Cell* getNeighbour(QVector<short> relativePosition) const;

    unsigned int countNeighbours(unsigned int filterState) const;
    unsigned int countNeighbours() const;

    static QVector<short> getRelativePosition(const QVector<unsigned int> cellPosition, const QVector<unsigned int> neighbourPosition);

private:
    unsigned int m_state; ///< Current state
    unsigned int m_nextState; ///< Temporary state, before validation

    QMap<QVector<short>, const Cell*> m_neighbours; ///< Cell's neighbours. Key is the relative position of the neighbour
};

#endif // CELL_H

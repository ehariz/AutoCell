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

    bool addNeighbour(const Cell* neighbour);
    QVector<const Cell*> getNeighbours() const;

private:
    unsigned int m_state; ///< Current state
    unsigned int m_nextState; ///< Temporary state, before validation

    QVector<const Cell*> m_neighbours; ///< Cell's neighbours
};

#endif // CELL_H

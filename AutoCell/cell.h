#ifndef CELL_H
#define CELL_H

#include <QVector>
#include <QDebug>

class Cell
{
public:
    Cell(unsigned int state = 0);

    void setState(unsigned int state);
    void validState();
    unsigned int getState() const;

    bool addNeighbour(const Cell* neighbour);
    QVector<const Cell*> getNeighbours() const;

private:
    unsigned int m_state; ///< Current state
    unsigned int m_nextState; ///< Temporary state, before validation

    QVector<const Cell*> m_neighbours; ///< Cell's neighbours
};

#endif // CELL_H

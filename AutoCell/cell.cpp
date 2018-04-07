#include "cell.h"

/** \fn Cell::Cell(unsigned int state)
 * \brief Constructs a cell with the state given. State 0 is dead state
 *
 * \param state Cell state, dead state by default
 */
Cell::Cell(unsigned int state):
    m_state(state), m_nextState(state)
{

}

/** \fn void Cell::setState(unsigned int state)
 * \brief Set temporary state
 *
 * To change current cell state, use setState(unsigned int state) then
 * validState().
 *
 * \param state New state
 */
void Cell::setState(unsigned int state)
{
    m_nextState = state;
}

/** \fn void Cell::validState()
 * \brief Validate temporary state
 *
 * To change current cell state, use setState(unsigned int state) then
 * validState().
 */
void Cell::validState()
{
    m_state = m_nextState;
}

/** \fn unsigned int Cell::getState() const
 * \brief Access current cell state
 */
unsigned int Cell::getState() const
{
    return m_state;
}

/** \fn bool Cell::addNeighbour(const Cell* neighbour)
 * \brief Add a new neighbour to the Cell
 *
 * \param neighbour New neighbour
 * \return False if the neighbour already exists
 */
bool Cell::addNeighbour(const Cell* neighbour)
{
    if (m_neighbours.count(neighbour))
        return false;
    m_neighbours.push_back(neighbour);
    return true;
}

/** \fn QVector<const Cell*> Cell::getNeighbours() const
 * \brief Access neighbours list
 */
QVector<const Cell*> Cell::getNeighbours() const
{
    return m_neighbours;
}

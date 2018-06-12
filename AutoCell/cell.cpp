#include "cell.h"

/** \brief Constructs a cell with the state given. State 0 is dead state
 *
 * \param state Cell state, dead state by default
 */
Cell::Cell(unsigned int state):
    m_nextState(state)
{
    m_states.push(state);
}

/** \brief Set temporary state
 *
 * To change current cell state, use setState(unsigned int state) then
 * validState().
 *(
 * \param state New state
 */
void Cell::setState(unsigned int state)
{
    m_nextState = state;
}

/** \brief Validate temporary state
 *
 * To change current cell state, use setState(unsigned int state) then
 * validState().
 */
void Cell::validState()
{
    m_states.push(m_nextState);
}

/** \brief Force the state change.
 *
 * Is equivalent to setState followed by validState
 *
 * \param state New state
 */
void Cell::forceState(unsigned int state)
{
    m_nextState = state;
    m_states.push(m_nextState);
}

/** \brief Access current cell state
 */
unsigned int Cell::getState() const
{
    return m_states.top();
}

/** \brief Set the previous state
 *
 * \return Return false if we are already at the first state
 */
bool Cell::back()
{
    if (m_states.size() <= 1)
        return false;
    m_states.pop();
    m_nextState = m_states.top();
    return true;
}

/** \brief Reset the cell to the 1st state
 */
void Cell::reset()
{
    while (m_states.size() > 2)
        m_states.pop();
    m_nextState = m_states.top();
}

/** \fn bool Cell::addNeighbour(const Cell* neighbour)
 * \brief Add a new neighbour to the Cell
 *
 * \param relativePosition Relative position of the new neighbour
 * \param neighbour New neighbour
 * \return False if the neighbour already exists
 */
bool Cell::addNeighbour(const Cell* neighbour, const QVector<short> relativePosition)
{
    if (m_neighbours.count(relativePosition))
        return false;

    m_neighbours.insert(relativePosition, neighbour);
    return true;
}

/** \brief Access neighbours list
 *
 * The map key is the relative position of the neighbour (like -1,0 for the cell just above)
 */
QMap<QVector<short>, const Cell *> Cell::getNeighbours() const
{
    return m_neighbours;
}

/** \brief Get the neighbour asked. If not existent, return nullptr
 */
const Cell *Cell::getNeighbour(QVector<short> relativePosition) const
{
    return m_neighbours.value(relativePosition, nullptr);
}

/** \brief Return the number of neighbour which have the given state
 */
unsigned int Cell::countNeighbours(unsigned int filterState) const
{
    unsigned int count = 0;
    for (QMap<QVector<short>, const Cell*>::const_iterator it = m_neighbours.begin(); it != m_neighbours.end(); ++it)
    {
        if ((*it)->getState() == filterState)
            count++;
    }
    return count;
}

/** \brief Return the number of neighbour which are not dead (=0)
 */
unsigned int Cell::countNeighbours() const
{
    unsigned int count = 0;
    for (QMap<QVector<short>, const Cell*>::const_iterator it = m_neighbours.begin(); it != m_neighbours.end(); ++it)
    {
        if ((*it)->getState() != 0)
            count++;
    }
    return count;
}

/** \brief Get the relative position, as neighbourPosition minus cellPosition
 *
 * \throw QString Different size of position vectors
 * \param cellPosition Cell Position
 * \param neighbourPosition Neighbour absolute position
 */
QVector<short> Cell::getRelativePosition(const QVector<unsigned int> cellPosition, const QVector<unsigned int> neighbourPosition)
{
    if (cellPosition.size() != neighbourPosition.size())
    {
        throw QString(QObject::tr("Different size of position vectors"));
    }
    QVector<short> relativePosition;
    for (short i = 0; i < cellPosition.size(); i++)
        relativePosition.push_back(neighbourPosition.at(i) - cellPosition.at(i));

    return relativePosition;
}

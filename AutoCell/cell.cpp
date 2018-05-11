#include "cell.h"

/** \brief Constructs a cell with the state given. State 0 is dead state
 *
 * \param state Cell state, dead state by default
 */
Cell::Cell(unsigned int state):
    m_state(state), m_nextState(state)
{

}

/** \brief Set temporary state
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

/** \brief Validate temporary state
 *
 * To change current cell state, use setState(unsigned int state) then
 * validState().
 */
void Cell::validState()
{
    m_state = m_nextState;
}

/** \brief Force the state change.
 *
 * Is equivalent to setState followed by validState
 *
 * \param state New state
 */
void Cell::forceState(unsigned int state)
{
    m_state = m_nextState = state;
}

/** \brief Access current cell state
 */
unsigned int Cell::getState() const
{
    return m_state;
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

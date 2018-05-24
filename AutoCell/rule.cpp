#include "rule.h"

bool Rule::inInterval(unsigned int i)
{
    if(i >= m_neighbourInterval.first && i<=m_neighbourInterval.second)
        return true;
    else
        return false;
}

Rule::Rule(QVector<unsigned int> currentCellValues, QPair<unsigned int, unsigned int> intervalNumber, QVector<unsigned int> intervalValues):
        m_currentCellPossibleValues(currentCellValues), m_neighbourInterval(intervalNumber), m_neighbourPossibleValues(intervalValues)
{

}

Rule::~Rule()
{

}

bool Rule::matchCell(const Cell & c)const
{
    unsigned int matchingNeighbours = 0;
    for(unsigned int i = 0; i < m_neighbourPossibleValues.size(); i++)
        matchingNeighbours += c.countNeighbours(m_neighbourPossibleValues.at(i));
    if(m_currentCellPossibleValues.contains(c.getState()) && inInterval(matchingNeighbours))
        return true; //the rule is applied to the cell
    else
        return false; //the rule is not applied to the cell
}

unsigned int Rule::getCellOutputState()const
{
        return m_cellOutputState;
}

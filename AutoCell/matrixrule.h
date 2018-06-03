#ifndef MATRIXRULE_H
#define MATRIXRULE_H

#include <QVector>
#include <QMap>
#include "cell.h"

QVector<unsigned int> fillInterval(unsigned int min, unsigned int max);

/** \brief Manage specific rules, about specific values of specific neighbour
 */
class MatrixRule
{
    public:
        MatrixRule(unsigned int finalState, QVector<unsigned int> currentStates = QVector<unsigned int>());


        virtual bool match(const Cell& cell);
        void addNeighbourState(QVector<short> relativePosition, unsigned int matchState);
        void addNeighbourState(QVector<short> relativePosition, QVector<unsigned int> matchStates);

    private:
        unsigned int m_finalState; ///< Final state
        QVector<unsigned int> m_currentStates; ///< Possibles current states

        QMap<QVector<short>,  QVector<unsigned int> > m_matrix; ///< Key correspond to relative position and the value to matchable states
};

#endif // MATRIXRULE_H

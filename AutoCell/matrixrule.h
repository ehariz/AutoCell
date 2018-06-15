#ifndef MATRIXRULE_H
#define MATRIXRULE_H

#include <QVector>
#include <QMap>
#include "cell.h"
#include "rule.h"

QVector<unsigned int> fillInterval(unsigned int min, unsigned int max);

/** \brief Manage specific rules, about specific values of specific neighbour
 */
class MatrixRule : public Rule
{
    public:
        MatrixRule(unsigned int finalState, QVector<unsigned int> currentStates = QVector<unsigned int>());


        virtual bool matchCell(const Cell* cell) const;
        virtual void addNeighbourState(QVector<short> relativePosition, unsigned int matchState);
        virtual void addNeighbourState(QVector<short> relativePosition, QVector<unsigned int> matchStates);

        QJsonObject toJson() const;


protected:

        QMap<QVector<short>,  QVector<unsigned int> > m_matrix; ///< Key correspond to relative position and the value to matchable states
};



#endif // MATRIXRULE_H

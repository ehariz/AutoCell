#ifndef CELLHANDLER_H
#define CELLHANDLER_H

#include <QString>
#include <QFile>
#include <QJsonDocument>
#include <QtWidgets>
#include <QMap>
#include <QRegExpValidator>

#include "cell.h"

class CellHandler
{
public:
    CellHandler(QString filename);
    virtual ~CellHandler();

    Cell* getCell(const QVector<unsigned int> position) const;

private:
    bool load(const QJsonObject &json);
    void foundNeighbours();
    void positionIncrement(QVector<unsigned int> &pos, unsigned int value = 1) const;
    QVector<QVector<unsigned int> > *getListNeighboursPositionsRecursive(const QVector<unsigned int> position, unsigned int dimension, QVector<unsigned int> lastAdd) const;
    QVector<QVector<unsigned int> > &getListNeighboursPositions(const QVector<unsigned int> position) const;

    QVector<unsigned int> m_dimensions; ///< Vector of x dimensions
    QMap<QVector<unsigned int>, Cell* > m_cells; ///< Map of cells, with a x dimensions vector as key
};

#endif // CELLHANDLER_H

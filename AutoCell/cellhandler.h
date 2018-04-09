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

private:
    bool load(const QJsonObject &json);

    QVector<unsigned int> m_dimensions; ///< Vector of x dimensions
    QMap<QVector<unsigned int>, Cell* > m_cells; ///< Map of cells, with a x dimensions vector as key
};

#endif // CELLHANDLER_H

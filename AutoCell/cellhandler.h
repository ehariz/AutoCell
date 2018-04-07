#ifndef CELLHANDLER_H
#define CELLHANDLER_H

#include <QString>
#include <QFile>
#include <QJsonDocument>
#include <QtWidgets>

class CellHandler
{
public:
    CellHandler(QString filename);

private:
    bool verifyJSON(const QJsonObject &json);
};

#endif // CELLHANDLER_H

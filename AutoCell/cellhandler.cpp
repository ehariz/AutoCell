#include "cellhandler.h"

CellHandler::CellHandler(QString filename)
{
    QFile loadFile(filename);
    if (!loadFile.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open given file.");
        throw QString(QObject::tr("Couldn't open given file"));
    }

    QJsonDocument loadDoc(QJsonDocument::fromJson(loadFile.readAll()));

    if (!verifyJSON(loadDoc.object()))
    {
        qWarning("File not valid");
        throw QString(QObject::tr("File not valid"));
    }

}

bool CellHandler::verifyJSON(const QJsonObject &json)
{
    if (!json.contains("height") || !json["height"].isDouble())
        return false;
    if (!json.contains("width") || !json["width"].isDouble())
        return false;
    if (!json.contains("cells") || !json["cells"].isArray())
        return false;
    QJsonArray lines = json["cells"].toArray();

    if (lines.size() != json["height"].toDouble())
        return false;
    for (unsigned int i = 0; i < lines.size(); i++)
    {
        if (!lines.at(i).isArray() || lines.at(i).toArray().size() != json["width"].toDouble())
            return false;
        for (unsigned int j = 0; j < lines.at(i).toArray().size(); j++)
        {
            if (!lines.at(i).toArray().at(j).isDouble())
                return false;
            if (lines.at(i).toArray().at(j).toDouble() < 0)
                return false;
        }
    }

    return true;

}

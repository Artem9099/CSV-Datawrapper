#ifndef DATAHANDLING_H
#define DATAHANDLING_H

#include <QFile>
#include <QTextStream>
#include <QtXml>
#include "globalcache.h"

class DataHandling
{
private:
    QFile file;
    QString fileTextCpy;
    QString newText;
    QStringList wordList;
    unsigned int posCounter;


public:
    DataHandling();
    void SetValueList(int resolution);
    void LoadFileContent();
    void LoadCsvContent();
    void LoadXmlContent();
};

#endif // DATAHANDLING_H

#ifndef CSVHANDLING_H
#define CSVHANDLING_H

#include <QFile>
#include <QTextStream>
#include "globalcache.h"

class CsvHandling
{
private:
    QFile file;
    QString fileTextCpy;
    QString newText;
    QStringList wordList;
    unsigned int posCounter;


public:
    CsvHandling();
    void SetValueList(int resolution);
    void LoadFileContent();
};

#endif // CSVHANDLING_H

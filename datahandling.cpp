#include "datahandling.h"

DataHandling::DataHandling()
{

}

void DataHandling::SetValueList(int resolution) {
    QTextStream textContent(&newText);
    double count = 0.0;
    dlTimeList.clear();
    dlPressureList.clear();
    wordList.clear();

    double x = double(posCounter) / 100 * double(resolution);
    double y = double(posCounter) - x;
    double z;
    if(y <= 0.0) {
        z = double(posCounter);
    }
    else {
        z = double(posCounter) / y;
    }

    while(!textContent.atEnd()) {
        QString line = textContent.readLine();
        if(count <= z) {
            count += 1.0;
            if(cDelimiter != ',') {
                // if the decimal separator is a ',' replace it with a '.'
                // But only if the delimiter is not a comma
                line.replace(",", ".");
            }
            wordList.append(line.split(cDelimiter));
            if(wordList.size() > 1) {
                dlTimeList << wordList[0].toFloat();
                dlPressureList << wordList[1].toFloat();
                // Define dMinVal and dMaxVal
                if(dlPressureList[dlPressureList.size() - 1] < dMinVal || dlPressureList.size() == 2) {
                    dMinVal = dlPressureList[dlPressureList.size() - 1];
                }
                if(dlPressureList[dlPressureList.size() - 1] > dMaxVal || dlPressureList.size() == 2) {
                    dMaxVal = dlPressureList[dlPressureList.size() - 1];
                }
            }
            wordList.clear();
        }
        else {
            count = count - z + 1;
        }
    }
    iPointsOut = dlTimeList.count();
}

void DataHandling::LoadFileContent() {
    if(sFilePath.right(3).toLower() == "xml") {
        LoadXmlContent();
    }
    else {
        LoadCsvContent();
    }
}

void DataHandling::LoadCsvContent() {
    file.setFileName(sFilePath);
    fileTextCpy = "";
    newText = "";

    if(file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        fileTextCpy = file.readAll();
        QTextStream fileContent(&fileTextCpy);
        file.close();
        posCounter = 0;

        // Remove needles from string
        while(!fileContent.atEnd()) {
            QString line = fileContent.readLine();
            if(line.contains("0") && !line.contains("a") && !line.contains("b") && !line.contains("c") && !line.contains("d") && !line.contains("e")
                    && !line.contains("f") && !line.contains("g") && !line.contains("h") && !line.contains("i") && !line.contains("j")
                    && !line.contains("k") && !line.contains("l") && !line.contains("m") && !line.contains("n") && !line.contains("o")
                    && !line.contains("p") && !line.contains("q") && !line.contains("r") && !line.contains("s") && !line.contains("t")
                    && !line.contains("u") && !line.contains("v") && !line.contains("w") && !line.contains("x") && !line.contains("y")
                    && !line.contains("z")) {
                newText.append(line + "\n");
                posCounter++;
            }
        }
    }
    iPointsTotal = posCounter;
}

void DataHandling::LoadXmlContent() {
    QDomDocument measureXml;
    file.setFileName(sFilePath);
    newText = "";

    if (file.open(QIODevice::ReadOnly))
    {
        measureXml.setContent(&file);
        file.close();
        posCounter = 0;

        QDomElement root = measureXml.documentElement();
        QDomElement node = root.firstChild().toElement();

        if(node.tagName() == "DATA"){
            while(!node.isNull()){
                QDomNodeList axis = node.childNodes();
                QString x = axis.item(0).toElement().text();
                QString y = axis.item(1).toElement().text();

                newText.append(x).append(cDelimiter).append(y).append("\n");
                node = node.nextSibling().toElement();
                posCounter++;
            }
        }
        node = node.nextSibling().toElement();
    }
    iPointsTotal = posCounter;
    //qDebug() << newText;
}

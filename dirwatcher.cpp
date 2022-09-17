#include "dirwatcher.h"
#include "qdebug.h"

DirWatcher::DirWatcher() {

}

void DirWatcher:: Watch() {
    watcher.addPath("C:\\Users\\Artem_Starovojt\\Desktop\\test");

    DirWatcher *mc = new DirWatcher();

    QStringList directoryList = watcher.directories();
        Q_FOREACH(QString directory, directoryList)
                qDebug() << "Directory name" << directory <<"\n";

        QObject::connect(&watcher, SIGNAL(directoryChanged(QString)), mc, SLOT(showModified(QString)));
}

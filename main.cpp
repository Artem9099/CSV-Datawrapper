#include "mainwindow.h"

#include <QApplication>

//QT_CHARTS_USE_NAMESPACE

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

//    QFileSystemWatcher watcher;
//    watcher.addPath("C:/Users/Artem_Starovojt/Desktop/test");

//    QStringList directoryList = watcher.directories();
//    Q_FOREACH(QString directory, directoryList)
//        qDebug() << "Directory name" << directory <<"\n";

//    MainWindow *mc = new MainWindow;

//    QObject::connect(&watcher, SIGNAL(directoryChanged(QString)), mc, SLOT(showModified(QString)));

    MainWindow w;
    w.show();
    return a.exec();
}

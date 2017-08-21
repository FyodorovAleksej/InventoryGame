#include "mainwindow.h"
#include <QApplication>
#include <QObject>
#include <QFile>
#include "dbadapter.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFile::copy(":/sound/appleSoundCut.mp3", "./appleSoundCut.mp3");

    DBAdapter* adapter = new DBAdapter();
    adapter->init();
    delete adapter;
    MainWindow w;
    w.show();
    return a.exec();
}

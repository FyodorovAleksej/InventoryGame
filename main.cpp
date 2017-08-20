#include "mainwindow.h"
#include <QApplication>
#include <QObject>
#include "dbadapter.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DBAdapter* adapter = new DBAdapter();
    adapter->init();
    delete adapter;
    MainWindow w;
    w.show();
    return a.exec();
}

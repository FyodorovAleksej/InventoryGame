#include "dbadapter.h"

DBAdapter::DBAdapter()
{
    sqliteDataBase = QSqlDatabase::addDatabase("QSQLITE");
    sqliteDataBase.setDatabaseName("mydatabase.sqlite");
    if (!sqliteDataBase.open())
    {
        qDebug() << sqliteDataBase.lastError().text();
    }
}

DBAdapter::~DBAdapter()
{
    if (this->isOpen())
    {
        this->close();
    }
}

void DBAdapter::close()
{
    if (this->isOpen())
    {
        this->sqliteDataBase.close();
        QSqlDatabase::removeDatabase("QSQLITE");
    }
}

bool DBAdapter::isOpen()
{
    return this->sqliteDataBase.isOpen();
}

QSqlQuery* DBAdapter::exec(QString sqlRequest)
{
    QSqlQuery* currentQuerry = new QSqlQuery();
    qDebug() << "exec : " << sqlRequest << "\n result - " << currentQuerry->exec(sqlRequest);
    return currentQuerry;
}

void DBAdapter::init()
{
    QSqlQuery* query = new QSqlQuery();
    query->exec("CREATE TABLE itemTable (picture VARCHAR(255), type VARCHAR(255) NOT NULL, sound VARCHAR(255), PRIMARY KEY(type))");
    query->exec("CREATE TABLE inventoryTable (id INTEGER PRIMARY KEY, count INTEGER, type VARCHAR(255), FOREIGN KEY(type) REFERENCES itemTable(type))");
    query->exec("INSERT INTO itemTable (picture, type, sound) VALUES (\":/images/appleImage.jpg\", \"Apple\", \"./appleSoundCut.mp3\");");
}

QSqlQuery* DBAdapter::getConnection()
{
    return new QSqlQuery();
}

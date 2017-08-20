#ifndef DBADAPTER_H
#define DBADAPTER_H

#include <QtSql/qtsqlglobal.h>
#include <QtSql/QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QDebug>

/**
 * @brief The DBAdapter class - the class for working with database (SQLite DB)
 */
class DBAdapter
{
public:
    /**
     * @brief DBAdapter - create adapter for connecting to database.
     */
    DBAdapter();
    /**
      * @brief ~DBAdapter - close connection and destroying this adapter
     */
    ~DBAdapter();

    /**
     * @brief getConnection - get connection with database for execute SQL requests
     * @return - query for sql requests
     */
    QSqlQuery* getConnection();

    /**
     * @brief isOpen - connection with dataBase is Open - ?
     * @return true - if connection is opened
     *             false - if conncetion is closed
     */
    bool isOpen();

    /**
     * @brief exec - execute this request in this database.
     * @param sqlRequest - the SQL request for execute by adapter
     * @return - query for getting result of execution
     */
    QSqlQuery* exec(QString sqlRequest);

    /**
     * @brief init - initializate database with primary data. Create tables and insert item (Apple Item)
     */
    void init();

    /**
     * @brief close - close connection with dataBase
     */
    void close();

private:
    QSqlDatabase sqliteDataBase;
};

#endif // DBADAPTER_H

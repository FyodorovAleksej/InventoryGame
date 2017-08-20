#ifndef NETSERVER_H
#define NETSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QMessageBox>
#include <QDebug>

/**
 * @brief The NetServer class - the class for working with sockets as a server
 */
class NetServer : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief NetServer - creating sever with this socket port
     * @param nPort - the socket port
     */
    NetServer(int nPort);
    ~NetServer();

signals:
    /**
     * @brief serverNewConnectionSignal - signal of new connection to server. Sending to Inventory
     * @param pClientSocket - the socket of connected client
     */
    serverNewConnectionSignal(QTcpSocket* pClientSocket);

public slots:
    /**
     * @brief slotNewConnection - the slot of nnew connection to server.
     */
    void slotNewConnection();
    /**
     * @brief close - close connection
     */
    void close();

private:
    QTcpServer* tcpServer;
};

#endif // NETSERVER_H

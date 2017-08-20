#ifndef NETCLIENT_H
#define NETCLIENT_H
#include <QObject>
#include <QTcpSocket>
#include <QMessageBox>

/**
 * @brief The NetClient class - the class of working with sockets as a client
 */
class NetClient : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief NetClient - create client with this ipAdress and socket port
     * @param strHost - the ipAdress of server
     * @param nPort - the port for socket
     */
    NetClient(const QString& strHost, int nPort);
    ~NetClient();

signals:
    /**
     * @brief readFromServerSignal - signal for reading to read server message from socket. Sending in Inventory
     * @param socket - the server socket
     */
    readFromServerSignal(QTcpSocket* socket);

public slots:
    /**
     * @brief slotReadyRead - slot of reading to read server message.
     */
    void slotReadyRead();

    /**
     * @brief slotError - the slot of error. Create QMessageBox with error message
     * @param err - the error message
     */
    void slotError(QAbstractSocket::SocketError err);

    /**
     * @brief close - close socket connection
     */
    void close();

private:
    QTcpSocket* connectedSocket;
};

#endif // NETCLIENT_H

#include "netclient.h"

NetClient::NetClient(const QString &strHost, int nPort)
{
    connectedSocket = new QTcpSocket(this);
    connectedSocket->connectToHost(strHost, nPort);
    connect(connectedSocket, SIGNAL(readyRead()), this, SLOT(slotReadyRead()));
    connect(connectedSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(slotError(QAbstractSocket::SocketError)));
}

NetClient::~NetClient()
{
    if (connectedSocket)
    {
        connectedSocket->close();
    }
}

void NetClient::slotReadyRead()
{
    emit readFromServerSignal(connectedSocket);
}

void NetClient::slotError(QAbstractSocket::SocketError err)
{
    QString strError = "Error: ";
    if (err == QAbstractSocket::HostNotFoundError)
        strError += "The host was not found.";
    if (err == QAbstractSocket::RemoteHostClosedError)
        strError += "The remote host is closed.";
    if (err == QAbstractSocket::ConnectionRefusedError)
        strError += "The connection was refused.";
    else
        strError += QString(connectedSocket->errorString());
    QMessageBox::information(0, "Client Error", "Unable to connect : " + strError);
}

void NetClient::close()
{
    this->connectedSocket->close();
}

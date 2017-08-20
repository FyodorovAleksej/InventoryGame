#include "netserver.h"

NetServer::NetServer(int nPort)
{
    tcpServer = new QTcpServer(this);
        if (!tcpServer->listen(QHostAddress::Any, nPort))
        {
            QMessageBox::critical(0, "Server Error", "Unable to start the server:" + tcpServer->errorString());
            tcpServer->close();
            return;
        }
        connect(tcpServer, SIGNAL(newConnection()), this, SLOT(slotNewConnection()));
}
NetServer::~NetServer()
{
    if (tcpServer)
    {
        tcpServer->close();
    }
    delete this->tcpServer;
}

void NetServer::slotNewConnection()
{
    QTcpSocket* pClientSocket = tcpServer->nextPendingConnection();
    emit serverNewConnectionSignal(pClientSocket);
    connect(pClientSocket, SIGNAL(disconnected()), pClientSocket, SLOT(deleteLater()));
}

void NetServer::close()
{
    this->tcpServer->close();
}

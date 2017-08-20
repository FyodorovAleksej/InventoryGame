#include "inventory.h"

Inventory::Inventory(int rows, int columns, QWidget *parent) : QTableWidget(rows, columns, parent)
{
    this->horizontalSize = rows;
    this->verticalSize = columns;

    inventoryCells = (InventoryCell***) malloc(sizeof(InventoryCell**) * verticalSize);
    for (int i = 0; i < horizontalSize; i++)
    {
        inventoryCells[i] = (InventoryCell**)malloc(sizeof(InventoryCell*) * horizontalSize);
        for (int j = 0; j < verticalSize; j++)
        {
            inventoryCells[i][j] = NULL;
            InventoryCell* cell = new InventoryCell(NULL, 0);
            this->setCell(cell, j, i);
        }
    }

    this->horizontalHeader()->setDefaultSectionSize(100);
    this->verticalHeader()->setDefaultSectionSize(100);

    this->horizontalHeader()->setVisible(false);
    this->verticalHeader()->setVisible(false);

    this->loadFromBD();
}

Inventory::~Inventory()
{
    for (int i = 0; i < verticalSize; i++)
    {
        for (int j = 0; j < horizontalSize; j++)
        {
            delete inventoryCells[i][j];
        }
        free(inventoryCells[i]);
    }
    free(inventoryCells);
}

InventoryCell* Inventory::getCell(int x, int y)
{
    if ((x >= 0 && x < horizontalSize) && (y >= 0 && y < verticalSize))
    {
        return inventoryCells[y][x];
    }
    return NULL;
}

void Inventory::setCell(InventoryCell *cell, int x, int y)
{
    if (inventoryCells[y][x] != NULL)
    {
        delete inventoryCells[y][x];
    }
    inventoryCells[y][x] = cell;
    this->setItem(y, x, cell);
}

void Inventory::dragEnterEvent(QDragEnterEvent *event)
{
    if (this->client == NULL)
    {
        if (event->mimeData()->hasFormat("text/csv"))
        {
            event->accept();
            qDebug() << "inventory enter drag";
        }
        else
        {
            event->ignore();
        }
    }
}

void Inventory::dragLeaveEvent(QDragLeaveEvent *event)
{
    event->accept();
}

void Inventory::dragMoveEvent(QDragMoveEvent *event)
{
    if (this->client == NULL)
    {
        this->repaintAllCells();
        QPoint target = getIndex(event->pos());
        if (target.x() >= 0)
        {
            getCell(target.x(), target.y())->setBackgroundColor(QColor(100, 100, 255, 100));
        }

        if (event->mimeData()->hasFormat("text/csv"))
        {
            event->setDropAction(Qt::MoveAction);
            event->accept();
        }
        else
        {
            event->ignore();
        }
    }
}

void Inventory::dropEvent(QDropEvent *event)
{
    if (this->client == NULL)
    {
        repaintAllCells();
        if (event->mimeData()->hasFormat("text/csv"))
        {
            QByteArray pieceData = event->mimeData()->data("text/csv");
            QDataStream dataStream(&pieceData, QIODevice::ReadOnly);
            QString message;
            dataStream >> message;
            if (message ==  "from inventory")
            {
                QPoint sourceCell;
                dataStream >> sourceCell;
                QPoint targetCell = this->getIndex(event->pos());
                if (targetCell.x() >= 0)
                {
                    if (!((targetCell.x() == sourceCell.x()) && (targetCell.y() == sourceCell.y())))
                    {
                        this->getCell(targetCell.x(), targetCell.y())->adding(this->getCell(sourceCell.x(), sourceCell.y()));
                        refreshCell(targetCell.x(), targetCell.y());
                        refreshCell(sourceCell.x(), sourceCell.y());
                    }
                }
            }
            else
            {
                if (message == "source")
                {
                    int itemCount;
                    QString itemType;
                    dataStream >> itemCount >> itemType;
                    QPoint target = this->getIndex(event->pos());
                    qDebug() << "from source. Count : " << itemCount << ". Type : " << itemType;
                    if (target.x() >= 0)
                    {
                        this->getCell(target.x(), target.y())->adding(new InventoryCell(InventoryItem::fromType(itemType), itemCount));
                        refreshCell(target.x(), target.y());
                    }
                }
            }

            event->setDropAction(Qt::MoveAction);
            event->accept();
        }
        else
        {
            event->ignore();
        }
    }
}

void Inventory::mousePressEvent(QMouseEvent *event)
{
    if (this->client == NULL)
    {
        if (event->button() == Qt::LeftButton)
        {
            QByteArray itemData;
            QDataStream dataStream(&itemData, QIODevice::WriteOnly);
            QPoint target = getIndex(event->pos());
            if (target.x() >= 0)
            {
                dataStream << QString("from inventory") << target;
                QMimeData *mimeData = new QMimeData;
                mimeData->setData("text/csv", itemData);

                QDrag *drag = new QDrag(this);
                drag->setPixmap(QPixmap(this->getCell(target.x(), target.y())->getPicture()));
                drag->setHotSpot(QPoint(event->pos().x() - target.x()*100, event->pos().y() - target.y()*100));
                drag->setMimeData(mimeData);

                if (!(drag->exec(Qt::MoveAction) == Qt::MoveAction))
                {
                }
            }
        }
        else
        {
            QPoint target = this->getIndex(event->pos());
            if (target.x() >= 0)
            {
                this->getCell(target.x(), target.y())->decrease();
                refreshCell(target.x(), target.y());
            }
        }
    }
}

void Inventory::saveToBD()
{
    qDebug() << "save to BD";
    DBAdapter *adapter = new DBAdapter();
    for (int j = 0; j < this->verticalSize; j++)
    {
        for (int i = 0; i < this->horizontalSize; i++)
        {
            QSqlQuery* query = adapter->getConnection();
            if (!query->exec("UPDATE inventoryTable SET type = \'" + this->getCell(i, j)->getType() + "\', count = " + QString::number(this->getCell(i, j)->getCount())+ "\' WHERE id = " + QString::number((j*this->horizontalSize) + i) + ";"))
            {
                query->exec("INSERT INTO inventoryTable (id, type, count) VALUES (" + QString::number((j*this->horizontalSize) + i) + ", \'" + this->getCell(i, j)->getType() + "\', " + QString::number(this->getCell(i, j)->getCount()) + ");");
            }
            delete query;
        }
    }
    adapter->close();
    delete adapter;
}

void Inventory::loadFromBD()
{
    qDebug() << "load from BD";
    DBAdapter *adapter = new DBAdapter();
    QSqlQuery* query = adapter->getConnection();
    if (!query->exec("SELECT * FROM inventoryTable"))
    {
        delete query;
        adapter->close();
        delete adapter;
        return;
    }
    QSqlRecord rec = query->record();
    int id = 0;
    QString type = "";
    int count = 0;
    if (query->first())
    {
        while (query->next())
        {
            id = query->value(rec.indexOf("id")).toInt();
            type = query->value(rec.indexOf("type")).toString();
            count = query->value(rec.indexOf("count")).toInt();
            this->setCell(new InventoryCell(InventoryItem::fromType(type), count), id % this->horizontalSize, id/this->horizontalSize);
        }
    }
    delete query;
    adapter->close();
    delete adapter;
    repaintAllCells();
}

void Inventory::refreshCell(int x, int y)
{
    DBAdapter *adapter = new DBAdapter();
    QSqlQuery* query = adapter->getConnection();
    if (!query->exec("UPDATE inventoryTable SET type = \'" + this->getCell(x, y)->getType() + "\', count = " + QString::number(this->getCell(x, y)->getCount()) + " WHERE id = " + QString::number((y*this->horizontalSize) + x) + ";"))
    {
        query->exec("INSERT INTO inventoryTable (id, type, count) VALUES (" + QString::number((y*this->horizontalSize) + x) + ", \'" + this->getCell(x, y)->getType() + "\', " + QString::number(this->getCell(x, y)->getCount()) + ");");
    }
    this->sendForAll(x, y);
    delete query;
    adapter->close();
    delete adapter;
}

QPoint Inventory::getIndex(QPoint pos)
{
    if ((pos.x() < this->horizontalSize * 100 + DELTA_X) && (pos.y() < verticalSize * 100 + DELTA_Y))
    {
        return QPoint((pos.x() - DELTA_X)/100, (pos.y() - DELTA_Y)/100);
    }
    return QPoint(-1, -1);
}

void Inventory::repaintAllCells()
{
    for (int i = 0; i < this->verticalSize; i++)
    {
        for (int j = 0; j < this->horizontalSize; j++)
        {
            getCell(j, i)->repaint();
        }
    }
}

void Inventory::newGame(QString mode)
{
    qDebug() << "start new game";
    for (int i = 0; i < verticalSize; i++)
    {
        for (int j = 0; j < horizontalSize; j++)
        {
            InventoryCell* cell = new InventoryCell(NULL, 0);
            this->setCell(cell, j, i);
        }
    }
    this->saveToBD();
    QStringList strings = mode.split('\t');


    if (this->client)
    {
        this->client->close();
        delete this->client;
    }
    if (this->server)
    {
        this->server->close();
        delete this->server;
        this->clientsList->clear();
        delete this->clientsList;
    }

    if (strings.at(0) == "server")
    {
        qDebug() << "server mode selected";
        this->server = new NetServer(SOCKETPORT);
        this->clientsList = new QList<QTcpSocket*>();
        connect(this->server, SIGNAL(serverNewConnectionSignal(QTcpSocket*)), this, SLOT(newServerConnection(QTcpSocket*)));
    }
    if (strings.at(0) == "client")
    {
        qDebug() << "client mode selected with address - " << strings.at(1);
        this->client = new NetClient(strings.at(1), SOCKETPORT);
        connect(this->client, SIGNAL(readFromServerSignal(QTcpSocket*)), this, SLOT(readFromServerSlot(QTcpSocket*)));
        this->clientNextBlockSize = 0;
    }
}

void Inventory::sendForAll()
{
    if (this->server != NULL)
    {
        for (QTcpSocket* socket : *(this->clientsList))
        {
            if (socket->isWritable())
            {
                this->sendInventory(socket);
            }
            else
            {
                clientsList->removeOne(socket);
            }
        }
    }
}

void Inventory::sendForAll(int x, int y)
{
    if (this->server != NULL)
    {
        for (QTcpSocket* socket : *(this->clientsList))
        {
            if (socket->isWritable())
            {
                this->sendOne(socket, x, y);
            }
            else
            {
                clientsList->removeOne(socket);
            }
        }
    }
}

void Inventory::sendInventory(QTcpSocket* socket)
{
    if (this->server)
    {
        qDebug() << "sending inventory from server";
        QByteArray  arrBlock;
        QDataStream out(&arrBlock, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_2);
        out << quint16(0);
        out << quint16(this->verticalSize * this->horizontalSize);
        for (int i = 0; i < this->verticalSize; i++)
        {
            for (int j = 0; j < this->horizontalSize; j++)
            {
                InventoryCell* cell = this->getCell(j, i);
                out << (i * this->horizontalSize + j) << cell->getCount() << cell->getType();
            }
        }
        out.device()->seek(0);
        out << quint16(arrBlock.size() - sizeof(quint16));

        socket->write(arrBlock);
    }
}

void Inventory::newServerConnection(QTcpSocket *socket)
{
    if (!clientsList->contains(socket))
    {
        qDebug() << "new connection to server";
        clientsList->append(socket);
        this->sendForAll();
    }
}
void Inventory::readFromServerSlot(QTcpSocket *socket)
{
    qDebug() << "reading from server";
    if (this->client)
    {
        QDataStream in(socket);
        in.setVersion(QDataStream::Qt_4_2);
        for (;;)
        {
            if (!clientNextBlockSize)
            {
                if (socket->bytesAvailable() < sizeof(quint16))
                {
                    break;
                }
                in >> clientNextBlockSize;
            }

            if (socket->bytesAvailable() < clientNextBlockSize)
            {
                break;
            }
            quint16 itemsCount(0);
            in >> itemsCount;
            int  id = 0;
            int count = 0;
            QString type = 0;
            if (itemsCount == 0)
            {
                InventoryItem::fromType(type)->removeAction();
            }
            else
            {
                for (int i = 0; i < itemsCount; i++)
                {
                    in >> id >> count >> type;
                    this->setCell(new InventoryCell(InventoryItem::fromType(type), count), id % this->horizontalSize, id/this->horizontalSize);
                }
            }
            this->repaintAllCells();
            this->saveToBD();
            this->clientNextBlockSize = 0;
        }
    }
}

void Inventory::sendOne(QTcpSocket* socket, int x, int y)
{
    if (this->server)
    {
        QByteArray  arrBlock;
        QDataStream out(&arrBlock, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_2);
        out << quint16(0);
        out << quint16(1);
        InventoryCell* cell = this->getCell(x, y);
        out << (y * this->horizontalSize + x) << cell->getCount() << cell->getType();
        out.device()->seek(0);
        out << quint16(arrBlock.size() - sizeof(quint16));
        socket->write(arrBlock);
    }
}

void Inventory::removeActionSendAll(int x, int y)
{
    if (this->server != NULL)
    {
        for (QTcpSocket* socket : *(this->clientsList))
        {
            if (socket->isWritable())
            {
                QByteArray  arrBlock;
                QDataStream out(&arrBlock, QIODevice::WriteOnly);
                out.setVersion(QDataStream::Qt_4_2);
                out << quint16(0);
                out << quint16(0);
                InventoryCell* cell = this->getCell(x, y);
                out << (y * this->horizontalSize + x) << cell->getCount() << cell->getType();
                out.device()->seek(0);
                out << quint16(arrBlock.size() - sizeof(quint16));
                socket->write(arrBlock);
            }
            else
            {
                clientsList->removeOne(socket);
            }
        }
    }
}



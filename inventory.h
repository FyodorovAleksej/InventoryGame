#ifndef INVENTORY_H
#define INVENTORY_H
#include <QObject>
#include <QTableWidget>
#include <QDragEnterEvent>
#include <QDragLeaveEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QMouseEvent>
#include <QMimeData>
#include <QDrag>
#include <QList>
#include <QDebug>
#include <QHeaderView>
#include "appleitem.h"
#include "dbadapter.h"
#include "inventorycell.h"
#include "netclient.h"
#include "netserver.h"
#define DELTA_X 0
#define DELTA_Y 0
#define SOCKETPORT 2323

/**
 * @brief The Inventory class - the class of inventory. Consists of cells (InventoryCell)
 */
class Inventory : public QTableWidget
{
    Q_OBJECT
public:
    /**
     * @brief Inventory - create inventory (QTableWidget) with this size. Filling this by cells (InventoryCell)
     * @param rows - the count of rows for inventory
     * @param columns - the count of columns for inventory
     * @param parent - the parent Widget
     */
    explicit Inventory(int rows, int columns, QWidget *parent = 0);
    ~Inventory();

    /**
     * @brief refreshCell - refreshing inventory's cell with this coordinates
     * @param x - x coordinate of cell
     * @param y - y coordinate of cell
     */
    void refreshCell(int x, int y);

    /**
     * @brief saveToBD - saving full Inventory to DataBase by adapter (DBAdapter).
     */
    void saveToBD();

    /**
     * @brief loadFromBD - load full Inventory from DataBase by adapter (DBAdapter)
     */
    void loadFromBD();

    /**
     * @brief newGame - reset Inventory for new game. Set mode of Inventory (server / client).
     * @param mode - the string with format: (("server")\("client" + "\t" + ipAdress))
     */
    void newGame(QString mode);

    /**
     * @brief sendInventory - send full Inventory by socket
     * @param socket - the socket for sending
     */
    void sendInventory(QTcpSocket* socket);

    /**
     * @brief sendForAll - send full inventory to all connected clients (clientsList)
     */
    void sendForAll();

    /**
     * @brief sendForAll - send cell with coordinates (x, y) of inventory to all connected alients (clientsList)
     * @param x - the x coordinate of cell
     * @param y - the y coordinate of cell
     */
    void sendForAll(int x, int y);

    /**
     * @brief sendOne - send one cell with coordinates (x, y) of inventory to one socket (client)
     * @param socket - the socket for sending
     * @param x - the x coordinate of cell
     * @param y - the y coordinate of cell
     */
    void sendOne(QTcpSocket* socket, int x, int y);

    /**
     * @brief removeActionSendAll - send action of remove (using) inventory item to all clients (clientsList)
     * @param x - the x coordinate of removed cell
     * @param y - the y coordinate of removed cell
     */
    void removeActionSendAll(int x, int y);

public slots:
    /**
     * @brief newServerConnection - action, that performed, when new connection to server was appear. (Added in clientsList). Used in Server Mode
     * @param socket - the socket of client
     */
    void newServerConnection(QTcpSocket* socket);

    /**
     * @brief readFromServerSlot - action, that performed, when socket was ready for rading by client. Used in Client Mode
     * @param socket - the socket for reading
     */
    void readFromServerSlot(QTcpSocket* socket);

protected:
    /**
     * @brief dragEnterEvent - override drag enter. Used in Server Mode
     * @param event - the event of enter drag
     */
    void dragEnterEvent(QDragEnterEvent *event) override;

    /**
     * @brief dragLeaveEvent - override drag leave. Used in Server Mode
     * @param event - the event of leave drag
     */
    void dragLeaveEvent(QDragLeaveEvent *event) override;

    /**
     * @brief dragMoveEvent - override drag move. Used in Server Mode
     * @param event - the event of move drag
     */
    void dragMoveEvent(QDragMoveEvent *event) override;

    /**
     * @brief dropEvent - override drop drag. Used in Server Mode. Parsing MimeData in format : (("from inventory" + QPoint)/("source" + count + type))
     * @param event - the event of drag drop
     */
    void dropEvent(QDropEvent *event) override;

    /**
     * @brief mousePressEvent - override mousePress. Used in Server Mode. Setting MimeData in format : ("from inventory" + QPoint). For Right Mouse Button - item->removeAction()  (playSound for appleItem)
     * @param event - the event of press mouse
     */
    void mousePressEvent(QMouseEvent *event) override;

private:
    /**
     * @brief getCell - getting inventory cell with this coordinates.
     * @param x - the x coordinate of cell
     * @param y - the y coordinate of cell
     * @return - the pointer on cell with this coordinates
     */
    InventoryCell* getCell(int x, int y);

    /**
     * @brief setCell - delete old cell and setting cell with this cell and this coordinates
     * @param cell - the new cell for setting
     * @param x - the x coordinates for setting
     * @param y - the y coordinates for setting
     */
    void setCell(InventoryCell* cell, int x, int y);

    /**
     * @brief getIndex - getting coordinates of cell, that contain this point
     * @param pos - the position of point in cell (or not)
     * @return (0 <= x < horizontalSize, 0 <= y < verticalSize), where x,y - coordinates of cell - if inventory contain this point
     *             (x = -1, y = -1) - if inventory don't contain this point
     */
    QPoint getIndex(QPoint pos);

    /**
     * @brief repaintAllCells - repaint all cells of Inventory
     */
    void repaintAllCells();

    int verticalSize;
    int horizontalSize;
    InventoryCell*** inventoryCells;

    NetServer* server = NULL;
    QList<QTcpSocket*> *clientsList;

    NetClient* client = NULL;
    quint16 clientNextBlockSize;
};

#endif // INVENTORY_H

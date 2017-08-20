#ifndef INVENTORYITEM_H
#define INVENTORYITEM_H

#include <QObject>
#include <QLabel>
#include <QDragEnterEvent>
#include <QDragLeaveEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QMouseEvent>
#include <QMimeData>
#include <QDrag>
#include <QDebug>

/**
 * @brief The InventoryItem class - abstract class of any item of inventory
 */
class InventoryItem : public QLabel
{
    Q_OBJECT
public:
    /**
     * @brief InventoryItem - can't create object - abstract class
     * @param parent - parent
     */
    explicit InventoryItem(QWidget *parent = NULL);

    /**
     * @brief getType - virtual method for getting type of inventory object
     * @return - type of inventory object
     */
    virtual QString getType() = 0;

    /**
     * @brief getPicturePath - virtual method for getting path of picture of this inventory object
     * @return - the path of picture file
     */
    virtual QString getPicturePath() = 0;

    /**
     * @brief removeAction - virtual method, that performed action of using item (playing sound)
     */
    virtual void removeAction() = 0;

    /**
     * @brief fromType - static method for getting new Inventory item by type
     * @param type - the type of object to create
     * @return - the new object of this type
     */
    static InventoryItem* fromType(QString type);

protected:
    /**
     * @brief dragEnterEvent - override drag enter.
     * @param event - the event of enter drag
     */
    void dragEnterEvent(QDragEnterEvent *event) override;

    /**
     * @brief dragLeaveEvent - override drag leave.
     * @param event - the event of leave drag
     */
    void dragLeaveEvent(QDragLeaveEvent *event) override;

    /**
     * @brief dragMoveEvent - override drag move.
     * @param event - the event of move drag
     */
    void dragMoveEvent(QDragMoveEvent *event) override;

    /**
     * @brief dropEvent - override drop drag.
     * @param event - the event of drag drop
     */
    void dropEvent(QDropEvent *event) override;

    /**
     * @brief mousePressEvent - override mousePress. Setting MimeData in format : ("source" + count + type).
     * @param event - the event of press mouse
     */
    void mousePressEvent(QMouseEvent *event) override;

    QString itemType;
    QString picturePath;
};

#endif // INVENTORYITEM_H

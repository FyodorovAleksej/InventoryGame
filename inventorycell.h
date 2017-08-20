#ifndef INVENTORYCELL_H
#define INVENTORYCELL_H
#include <QTableWidgetItem>
#include "inventoryitem.h"

/**
 * @brief The InventoryCell class - cells of Inventory. Contains information about Inventory cells (item/count)
 */
class InventoryCell : public QTableWidgetItem
{
public:
    /**
     * @brief InventoryCell - create cell of this item with this count
     * @param item - the item of cell
     * @param itemCount - the count of this item
     */
    InventoryCell(InventoryItem* item = NULL, int itemCount = 0);
    ~InventoryCell();

    /**
     * @brief getType - getting type of item of this cell
     * @return - the type of cell's item
     */
    QString getType();

    /**
     * @brief getCount - getting count of item of this cell
     * @return - the count of cell's item
     */
    int getCount();

    /**
     * @brief getPicture - getting local path of file with picture of cell's item
     * @return - the local path of cell's item's picture
     */
    QString getPicture();

    /**
     * @brief isEmpty - is this cell empty?
     * @return - true - if this item empty (count = 0 || item = NULL)
     *               false - if this item don't empty
     */
    bool isEmpty();

    /**
     * @brief clear - clear this item. Delete old item of cell and set count = 0;
     */
    void clear();

    /**
     * @brief setItem - setting new item to this cell
     * @param item - the new item for this cell
     */
    void setItem(InventoryItem* item);

    /**
     * @brief adding - adding other cell, if types are equals. Increased this count
     * @param cell - the other cell for adding
     * @return - did adding finished successfully?
     */
    bool adding(InventoryCell *cell);

    /**
     * @brief increase - increasing count of cell's item
     * @param value - the value for increasing
     * @return - did increasing finished succesfully?
     */
    bool increase(int value = 1);

    /**
     * @brief decrease - decreasing count of cell's item
     * @param value - the value for decreasing
     * @return did decreasing finished succesfully?
     */
    bool decrease(int value = 1);

    /**
     * @brief repaint - repaint this cell. Repaint picture and count
     */
    void repaint();

private:
    InventoryItem* item;
    int itemCount;
};

#endif // INVENTORYCELL_H

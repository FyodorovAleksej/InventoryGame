#ifndef APPLEITEM_H
#define APPLEITEM_H

#include <QObject>
#include <QtMultimedia/QMediaPlayer>
#include "dbadapter.h"
#include "inventoryitem.h"

/**
 * @brief The AppleItem class - class of Apple, that describes apple label in main window.
 * inherits InventoryItem.
 */
class AppleItem : public InventoryItem
{
public:
    /**
     * @brief AppleItem - create AppleItem - new Label with Apple
     * @param parent - the parent widget
     */
    AppleItem(QWidget* parent = NULL);

    /**
     * @brief removeAction - the action, that was performed when apple was used in Incentory (right mouse button). Played sound of apple.
     */
    void removeAction();

    /**
     * @brief getPicturePath - getting path of picture for apple. Getting that from DataBase.
     * @return - the local path of picture of item
     */
    QString getPicturePath();

    /**
     * @brief getType - getting type of item - "Apple"
     * @return - "Apple"
     */
    QString getType();

    /**
     * @brief getSound - getting local path of file with apple sound. Getting that from DataBase
     * @return - the local path of sound file
     */
    QString getSound();

};

#endif // APPLEITEM_H

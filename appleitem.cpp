#include "appleitem.h"

AppleItem::AppleItem(QWidget* parent) : InventoryItem(parent)
{
    this->itemType =  "Apple";
    this->setAttribute(Qt::WA_DeleteOnClose);
    qDebug() << "end Apple";
}

void AppleItem::removeAction()
{
    QMediaPlayer* player = new QMediaPlayer();
    player->setMedia(QMediaContent(QUrl::fromLocalFile(this->getSound())));
    player->play();
}

QString AppleItem::getType()
{
    return this->itemType;
}

QString AppleItem::getPicturePath()
{
    DBAdapter* adapter = new DBAdapter();
    QSqlQuery* query = adapter->exec("SELECT picture from itemTable WHERE type = \'" + this->getType() + "\';");
    QString picture = "";
    if (query->first())
    {
        picture = query->value(0).toString();
    }
    delete query;
    adapter->close();
    return picture;
}

QString AppleItem::getSound()
{
    DBAdapter* adapter = new DBAdapter();
    QSqlQuery* query = adapter->exec("SELECT sound from itemTable WHERE type = \'" + this->getType() + "\';");
    QString sound = "";
    if (query->first())
    {
        sound = query->value(0).toString();
        qDebug() << "getSound - " << sound;
    }
    delete query;
    adapter->close();
    return sound;
}

#include "inventoryitem.h"
#include "appleitem.h"

InventoryItem::InventoryItem(QWidget *parent) : QLabel(parent)
{

}

void InventoryItem::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat("text/csv"))
        event->accept();
    else
        event->ignore();
    qDebug() << "Enter";
}

void InventoryItem::dragLeaveEvent(QDragLeaveEvent *event)
{
    event->accept();
    qDebug() << "Leave";
}

void InventoryItem::dragMoveEvent(QDragMoveEvent *event)
{
    if (event->mimeData()->hasFormat("text/csv"))
    {
        event->setDropAction(Qt::MoveAction);
        event->accept();
    }
    else
    {
        event->ignore();
    }
    qDebug() << "Move";
}

void InventoryItem::dropEvent(QDropEvent *event)
{
    if (event->mimeData()->hasFormat("text/csv"))
    {
        event->ignore();
    }
    qDebug() << "Drop";
}

void InventoryItem::mousePressEvent(QMouseEvent *event)
{
    QByteArray itemData;
    QDataStream dataStream(&itemData, QIODevice::WriteOnly);
    int count = 1;
    dataStream << QString("source") << count << this->getType();

    QMimeData *mimeData = new QMimeData();
    mimeData->setData("text/csv", itemData);

    qDebug() << "MousePress";
    QDrag *drag = new QDrag(this);
    drag->setPixmap(QPixmap(this->getPicturePath()));
    drag->setHotSpot(QPoint(event->pos().x(), event->pos().y() - this->geometry().center().y() + drag->pixmap().size().height()/2));
    drag->setMimeData(mimeData);

    if (!(drag->exec(Qt::MoveAction) == Qt::MoveAction))
    {

    }
}

InventoryItem* InventoryItem::fromType(QString type)
{
    if (type == "Apple")
        return new AppleItem();
    return NULL;
}

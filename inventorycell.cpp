#include "inventorycell.h"

InventoryCell::InventoryCell(InventoryItem *item, int count)
{
    this->item = item;
    this->itemCount = count;
    this->setTextAlignment(Qt::AlignBottom | Qt::AlignRight);
}

InventoryCell::~InventoryCell()
{
    delete this->item;
}

QString InventoryCell::getType()
{
    if (!this->isEmpty())
    {
        return this->item->getType();
    }
    return QString();
}

int InventoryCell::getCount()
{
    return this->itemCount;
}

QString InventoryCell::getPicture()
{
    if (this->item)
    {
        return this->item->getPicturePath();
    }
    return QString("");
}

bool InventoryCell::isEmpty()
{
    return (this->itemCount == 0 || this->item == NULL);
}

bool InventoryCell::increase(int value)
{
    if (!this->isEmpty())
    {
        this->itemCount += value;
        return true;
    }
    return false;
}

bool InventoryCell::decrease(int value)
{
    if (!this->isEmpty())
    {
        this->itemCount -= value;
        item->removeAction();
        if (this->itemCount <= 0)
        {
            this->clear();
        }
        this->repaint();
        return true;
    }
    return false;
}

void InventoryCell::clear()
{
    this->itemCount = 0;
    delete this->item;
    this->item = NULL;
}

void InventoryCell::setItem(InventoryItem *item)
{
    this->item = item;
}

void InventoryCell::repaint()
{
    if (this->item != NULL)
    {
        QImage *img = new QImage();
        if (img->load(this->item->getPicturePath()))
        {
            this->setData(Qt::BackgroundColorRole, QPixmap::fromImage(*img));
        }
        if (this->itemCount > 0)
        {
            this->setText(QString::number(this->itemCount));
        }
        else
        {
            this->setText("");
        }
    }
    else
    {
        this->setText("");
        this->setBackgroundColor(QColor(255, 255, 255, 0));
    }
}

bool InventoryCell::adding(InventoryCell *cell)
{
    if (this->getType() == cell->getType())
    {
        this->increase(cell->getCount());
        cell->itemCount = 0;
        delete cell->item;
        cell->item = NULL;
        this->repaint();
        cell->repaint();
        return true;
    }
    else
    {
        if (this->item == NULL)
        {
            this->setItem(cell->item);
            this->itemCount = cell->getCount();
            cell->itemCount = 0;
            cell->item = NULL;
            cell->repaint();
            this->repaint();
            return true;
        }
    }
    return false;
}

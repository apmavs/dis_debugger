#include "WatchDatumItem.h"

WatchDatumItem::WatchDatumItem(QString categoryName)
    : EntityDatumItem(categoryName)
{
}

WatchDatumItem::WatchDatumItem(EntityDatumItem* parent, const DatumInfo* d)
    : EntityDatumItem(parent, d)
{
}

WatchDatumItem* WatchDatumItem::createWatchItem(EntityDatumItem* fromThis)
{
    WatchDatumItem* newItem;
    const DatumInfo* datum = fromThis->getWatchedDatum();
    if(datum != NULL)
    {
        newItem = new WatchDatumItem(NULL, datum);
        newItem->setToolTip(0, QString(datum->getDescription().c_str()));
        newItem->setToolTip(1, QString(datum->getDescription().c_str()));
        newItem->setToolTip(2, QString(datum->getDescription().c_str()));
    }
    else
        newItem = new WatchDatumItem(fromThis->getCategoryName());

    // Create copies of all children
    for(int kidIdx = 0; kidIdx < fromThis->childCount(); kidIdx++)
    {
        EntityDatumItem* kid = (EntityDatumItem*)(fromThis->child(kidIdx));
        WatchDatumItem* newKid = createWatchItem(kid);
        newItem->addChild(newKid);
    }

    return newItem;
}


WatchDatumItem::~WatchDatumItem()
{
}

void WatchDatumItem::setDisplay()
{
    mutex.lock();
    if(watched_datum == NULL)
    {   // category item
        setText(0, category_name);
    }
    else
    {
        QString entityName(watched_datum->getEntityName().c_str());
        setText(0, entityName);
        setText(1, watched_datum->getName().c_str());
        std::string curVal = watched_datum->getValue() + " ";
        curVal += watched_datum->getUnit();
        setText(2, curVal.c_str());
    }
    setColor(2);
    mutex.unlock();
}



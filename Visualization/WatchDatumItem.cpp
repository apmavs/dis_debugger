#include "WatchDatumItem.h"

WatchDatumItem::WatchDatumItem(EntityDatumItem* fromThis)
    : EntityDatumItem(fromThis->getCategoryName())
{
    watched_datum = fromThis->getWatchedDatum();

    // Create copies of all children
    for(int kidIdx = 0; kidIdx < fromThis->childCount(); kidIdx++)
    {
        EntityDatumItem* kid = (EntityDatumItem*)(fromThis->child(kidIdx));
        WatchDatumItem* newKid = new WatchDatumItem(kid);
        addChild(newKid);
    }

    if(watched_datum != NULL)
    {
        setToolTip(0, QString(watched_datum->getDescription().c_str()));
        setToolTip(1, QString(watched_datum->getDescription().c_str()));
        setToolTip(2, QString(watched_datum->getDescription().c_str()));
    }
}


WatchDatumItem::~WatchDatumItem()
{
}

void WatchDatumItem::setDisplay()
{
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
}



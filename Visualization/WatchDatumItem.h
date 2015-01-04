#ifndef WATCHDATUMITEM_H
#define WATCHDATUMITEM_H

#include "EntityDatumItem.h"

#include <QTreeWidgetItem>

class WatchDatumItem : public EntityDatumItem
{
private:
    WatchDatumItem();
    WatchDatumItem(const WatchDatumItem& rhs);
    WatchDatumItem& operator=(const WatchDatumItem& rhs);
    WatchDatumItem(QString categoryName);
    WatchDatumItem(EntityDatumItem* parent, const DatumInfo* d);

private slots:
    void dropItem(QTreeWidgetItem* item);
    virtual void setDisplay();

public:
    static WatchDatumItem* createWatchItem(EntityDatumItem* fromThis);
    virtual ~WatchDatumItem();
};

#endif // WATCHDATUMITEM_H

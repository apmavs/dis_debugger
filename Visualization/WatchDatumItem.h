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

private slots:
    void dropItem(QTreeWidgetItem* item);

protected:
    virtual void setDisplay();

public:
    WatchDatumItem(EntityDatumItem* fromThis);
    virtual ~WatchDatumItem();
};

#endif // WATCHDATUMITEM_H

#ifndef WATCHDATUMITEM_H
#define WATCHDATUMITEM_H

#include "EntityDatumItem.h"

#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QComboBox>

class WatchDatumItem : public EntityDatumItem
{
private:
    QComboBox *unitBox;

    WatchDatumItem();
    WatchDatumItem(const WatchDatumItem& rhs);
    WatchDatumItem& operator=(const WatchDatumItem& rhs);
    WatchDatumItem(EntityDatumItem* parent, QString categoryName);
    WatchDatumItem(EntityDatumItem* parent, const DatumInfo* d);

private slots:
    void dropItem(QTreeWidgetItem* item);
    virtual void setDisplay();

public:
    static WatchDatumItem* createWatchItem(EntityDatumItem* fromThis);
    static WatchDatumItem* createFromStringRepresentation(QString rep,
                                       WatchDatumItem* parent = NULL);

    virtual ~WatchDatumItem();
    virtual QString getStringRepresentation() const;
    virtual void addUnitSelectionBox(QTreeWidget *parentTree);
};

#endif // WATCHDATUMITEM_H

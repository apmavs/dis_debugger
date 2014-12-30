#ifndef ENTITYDATUMITEM_H
#define ENTITYDATUMITEM_H

#include "DatumItem.h"

#include <QTreeWidgetItem>

class EntityDatumItem : public QTreeWidgetItem, public DatumItem
{
protected:
    virtual void setDisplay();
    virtual void clearDisplay();

public:
    EntityDatumItem(QString categoryName); // For category items with no datum
    EntityDatumItem(EntityDatumItem* parent, const DatumInfo* d);
    virtual ~EntityDatumItem();

    // Call with pointer to caller
    virtual void activate(const void* widgetPtr);    // Update whenever datum changes
    virtual void deactivate(const void* widgetPtr);  // Stop updating with datum changes
};

#endif // ENTITYDATUMITEM_H

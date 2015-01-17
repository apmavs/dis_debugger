#ifndef ENTITYDATUMITEM_H
#define ENTITYDATUMITEM_H

#include "DatumItem.h"

#include <QTreeWidgetItem>

class EntityDatumItem : public QTreeWidgetItem, public DatumItem
{
private:
    EntityDatumItem();
    EntityDatumItem(const EntityDatumItem& rhs);
    EntityDatumItem& operator=(const EntityDatumItem& rhs);

private slots:
    // These are slots to ensure GUI updates are done in GUI thread
    virtual void setDisplay();
    virtual void clearDisplay();

public:
    EntityDatumItem(QString categoryName); // For category items with no datum
    EntityDatumItem(EntityDatumItem* parent, const DatumInfo* d);
    virtual ~EntityDatumItem();

    QString getName();

    // Call with pointer to caller
    virtual void activate(const void* widgetPtr);    // Update whenever datum changes
    virtual void deactivate(const void* widgetPtr);  // Stop updating with datum changes
};

#endif // ENTITYDATUMITEM_H

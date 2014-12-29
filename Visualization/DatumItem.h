#ifndef DATUMITEM_H
#define DATUMITEM_H

#include "DatumInfo.h"
#include "DataModelController.h"

#include <QTreeWidgetItem>
#include <set>

class DatumItem : public QTreeWidgetItem, public DatumObserver
{
private:
    DataModelController* controller;
    std::set<const void*> interested_widgets;
    const DatumInfo* watched_datum;
    QString category_name;

    std::string entity_removed;
    const DatumInfo* new_datum;

protected:
    QString getCategoryName();
    void setDisplay();
    void clearDisplay();

public:
    DatumItem(QString categoryName); // For category items with no datum
    DatumItem(DatumItem* parent, const DatumInfo* d);
    virtual ~DatumItem();

    // Satisfy DatumObserver interface
    virtual void notifyNewDatum(const DatumInfo* datum);
    virtual void notifyNewValue(const DatumInfo* datum);
    virtual void notifyEntityRemoved(std::string entity);
    virtual void notifyAllDatumsInvalid();

    // Call with pointer to caller
    void activate(const void* widgetPtr);    // Update whenever datum changes
    void deactivate(const void* widgetPtr);  // Stop updating with datum changes
};

#endif // DATUMITEM_H

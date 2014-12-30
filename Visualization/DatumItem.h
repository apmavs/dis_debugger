#ifndef DATUMITEM_H
#define DATUMITEM_H

#include "DatumInfo.h"
#include "DataModelController.h"

#include <set>

class DatumItem : public DatumObserver
{
protected:
    DataModelController* controller;
    std::set<const void*> interested_widgets;
    const DatumInfo* watched_datum;
    QString category_name;
    std::string entity_removed;
    const DatumInfo* new_datum;

    DatumItem(QString categoryName); // For category items with no datum
    DatumItem(const DatumInfo* d);
    QString getCategoryName();
    virtual void setDisplay() = 0;
    virtual void clearDisplay() = 0;

public:
    virtual ~DatumItem();

    // Satisfy DatumObserver interface
    virtual void notifyNewDatum(const DatumInfo* datum);
    virtual void notifyNewValue(const DatumInfo* datum);
    virtual void notifyEntityRemoved(std::string entity);
    virtual void notifyAllDatumsInvalid();

    // Call with pointer to caller
    virtual void activate(const void* widgetPtr);    // Update whenever datum changes
    virtual void deactivate(const void* widgetPtr);  // Stop updating with datum changes
};

#endif // DATUMITEM_H

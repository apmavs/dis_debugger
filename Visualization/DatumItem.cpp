#include "DatumItem.h"

// For category items with no datum
DatumItem::DatumItem(QString categoryName)
{
    controller = DataModelController::getInstance();
    interested_widgets.clear();
    watched_datum = NULL;
    category_name = categoryName;
}

DatumItem::DatumItem(const DatumInfo* d)
{
    controller = DataModelController::getInstance();
    interested_widgets.clear();
    watched_datum = d;
    category_name = d->getCategory().c_str();
}

DatumItem::~DatumItem()
{
    if(interested_widgets.size() > 0)
        controller->unregisterDatumObserver(this, watched_datum);
}

const DatumInfo* DatumItem::getWatchedDatum()
{
    return watched_datum;
}

QString DatumItem::getCategoryName()
{
    return category_name;
}

// Track how many widgets are displaying the item

void DatumItem::notifyNewDatum(const DatumInfo* datum)
{
    new_datum = datum;
}

void DatumItem::notifyNewValue(const DatumInfo* datum)
{
    watched_datum = datum;
    setDisplay();
}

void DatumItem::notifyEntityRemoved(std::string entity)
{
    entity_removed = entity;
}

void DatumItem::notifyAllDatumsInvalid()
{
    watched_datum = NULL;
    new_datum = NULL;
    entity_removed = "";
}

// Register for datum updates if anybody is displaying
void DatumItem::activate(const void* widgetPtr)
{
    if(interested_widgets.count(widgetPtr) == 0)
    {
        interested_widgets.insert(widgetPtr);
        if(interested_widgets.size() == 1) // first interest
        {
            if(watched_datum != NULL)
                controller->registerDatumObserver(this, watched_datum);
            setDisplay();
        }
    }
}

// Track how many widgets are displaying the item
// Unregister for datum updates if nobody is displaying
void DatumItem::deactivate(const void* widgetPtr)
{
    if(interested_widgets.count(widgetPtr) != 0)
    {
        interested_widgets.erase(widgetPtr);
        if(interested_widgets.size() == 0) // nobody interested
        {
            if(watched_datum != NULL)
                controller->unregisterDatumObserver(this, watched_datum);
            clearDisplay();
        }
    }
}




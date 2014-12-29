#include "DatumItem.h"

// For category items with no datum
DatumItem::DatumItem(QString categoryName)
    : QTreeWidgetItem(QStringList(categoryName))
{
    controller = DataModelController::getInstance();
    interested_widgets.clear();
    watched_datum = NULL;
    category_name = categoryName;
    clearDisplay();
}

DatumItem::DatumItem(DatumItem* parent, const DatumInfo* d)
    : QTreeWidgetItem(parent)
{
    controller = DataModelController::getInstance();
    interested_widgets.clear();
    watched_datum = d;
    setToolTip(0, QString(watched_datum->getDescription().c_str()));
    setToolTip(1, QString(watched_datum->getDescription().c_str()));

    if(parent != NULL)
        category_name = parent->getCategoryName();
    else
        category_name = d->getCategory().c_str();

    clearDisplay();
}

DatumItem::~DatumItem()
{
    if(interested_widgets.size() > 0)
        controller->unregisterDatumObserver(this, watched_datum);
}

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


QString DatumItem::getCategoryName()
{
    return category_name;
}

void DatumItem::setDisplay()
{
    if(watched_datum == NULL)
    {   // category item
        setText(0, category_name);
    }
    else
    {
        setText(0, watched_datum->getName().c_str());
        std::string curVal = watched_datum->getValue() + " ";
        curVal += watched_datum->getUnit();
        setText(1, curVal.c_str());
    }
}

void DatumItem::clearDisplay()
{
    if(watched_datum != NULL)
    {
        setText(0, "COLLAPSED");
    }
}

// Track how many widgets are displaying the item
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

        // If we have childeren, activate them too
        for(int kidIdx = 0; kidIdx < childCount(); kidIdx++)
        {
            DatumItem* kid = (DatumItem*)(child(kidIdx));
            kid->activate(widgetPtr);
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

        // If we have childeren, deactivate them too
        for(int kidIdx = 0; kidIdx < childCount(); kidIdx++)
        {
            DatumItem* kid = (DatumItem*)(child(kidIdx));
            kid->deactivate(widgetPtr);
        }
    }
}




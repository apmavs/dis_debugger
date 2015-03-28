#include "DatumItem.h"

// For category items with no datum
DatumItem::DatumItem(QString categoryName)
    : QObject(),
      watched_datum(NULL),
      unit_class(NULL)
{
    controller = DataModelController::getInstance();
    interested_widgets.clear();
    category_name = categoryName;

    QObject::connect(this, SIGNAL(updateDisplay()), this, SLOT(setDisplay()),
            Qt::QueuedConnection);
    QObject::connect(this, SIGNAL(stopUpdates()), this, SLOT(clearDisplay()),
            Qt::QueuedConnection);
}

DatumItem::DatumItem(const DatumInfo* d, bool constantUpdates)
    : QObject(),
      watched_datum(d),
      constant_updates(constantUpdates)
{
    controller = DataModelController::getInstance();
    unit_class = controller->getUnitClassDef(watched_datum->getUnitClass());
    interested_widgets.clear();
    category_name = d->getCategory().c_str();

    QObject::connect(this, SIGNAL(updateDisplay()), this, SLOT(setDisplay()),
            Qt::QueuedConnection);
    QObject::connect(this, SIGNAL(stopUpdates()), this, SLOT(clearDisplay()),
            Qt::QueuedConnection);
}

DatumItem::~DatumItem()
{
    if(watched_datum != NULL)
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

void DatumItem::notifyNewDatum(const DatumInfo*)
{
}

void DatumItem::notifyNewValue(const DatumInfo*)
{
    emit updateDisplay();
}

void DatumItem::notifyEntityRemoved(std::string)
{
}

void DatumItem::notifyAllDatumsInvalid()
{
}

// Register for datum updates if anybody is displaying
void DatumItem::activate(const void* widgetPtr)
{
    mutex.lock();
    if(interested_widgets.count(widgetPtr) == 0)
    {
        interested_widgets.insert(widgetPtr);
        if(interested_widgets.size() == 1) // first interest
        {
            if(watched_datum != NULL)
            {
                controller->registerDatumObserver(this,
                                                  watched_datum,
                                                  constant_updates);
            }
            emit updateDisplay();
        }
    }
    mutex.unlock();
}

// Track how many widgets are displaying the item
// Unregister for datum updates if nobody is displaying
void DatumItem::deactivate(const void* widgetPtr)
{
    mutex.lock();
    if(interested_widgets.count(widgetPtr) != 0)
    {
        interested_widgets.erase(widgetPtr);
        if(interested_widgets.size() == 0) // nobody interested
        {
            if(watched_datum != NULL)
                controller->unregisterDatumObserver(this, watched_datum);
            emit stopUpdates();
        }
    }
    mutex.unlock();
}

bool DatumItem::equivalentTo(const DatumItem* rhs) const
{
    bool ret = true;

    if(watched_datum == NULL)
    {
        if(rhs->watched_datum == NULL)
            ret = (category_name == rhs->category_name);
        else
            ret = false;
    }
    else
    {
        if(rhs->watched_datum == NULL)
            ret = false;
        else
            ret = watched_datum->equivalentTo(rhs->watched_datum);
    }

    return ret;
}




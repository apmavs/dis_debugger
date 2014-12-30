#include "EntityDatumItem.h"

EntityDatumItem::EntityDatumItem(QString categoryName)
    : QTreeWidgetItem(QStringList(categoryName)),
      DatumItem(categoryName)
{
    clearDisplay();
}

EntityDatumItem::EntityDatumItem(EntityDatumItem* parent, const DatumInfo* d)
    : QTreeWidgetItem(parent), DatumItem(d)
{
    clearDisplay();
    setToolTip(0, QString(d->getDescription().c_str()));
    setToolTip(1, QString(d->getDescription().c_str()));
}

EntityDatumItem::~EntityDatumItem()
{
}

void EntityDatumItem::setDisplay()
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

void EntityDatumItem::clearDisplay()
{
    if(watched_datum != NULL)
        setText(0, "COLLAPSED");
}

void EntityDatumItem::activate(const void *widgetPtr)
{
    if(interested_widgets.count(widgetPtr) == 0)
    {
        DatumItem::activate(widgetPtr);

        // If we have childeren, activate them too
        for(int kidIdx = 0; kidIdx < childCount(); kidIdx++)
        {
            EntityDatumItem* kid = (EntityDatumItem*)(child(kidIdx));
            kid->activate(widgetPtr);
        }
    }
}

void EntityDatumItem::deactivate(const void* widgetPtr)
{
    if(interested_widgets.count(widgetPtr) != 0)
    {
        DatumItem::deactivate(widgetPtr);

        // If we have childeren, deactivate them too
        for(int kidIdx = 0; kidIdx < childCount(); kidIdx++)
        {
            EntityDatumItem* kid = (EntityDatumItem*)(child(kidIdx));
            kid->deactivate(widgetPtr);
        }
    }
}

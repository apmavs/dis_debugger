#include "EntityDataList.h"


EntityDataList::EntityDataList(QWidget *parent) :
    QTreeWidget(parent),
    active_filter("")
{
    controller = DataModelController::getInstance();
    controller->registerObserver(this);
    active_entity = "";
    connect(this, SIGNAL(expanded(QModelIndex)), this, SLOT(activateItem(QModelIndex)));
    connect(this, SIGNAL(collapsed(QModelIndex)), this, SLOT(deactivateItem(QModelIndex)));
}

void EntityDataList::addItem(const DatumInfo* datum)
{
    QString catStr = QString(datum->getCategory().c_str());
    EntityDatumItem* catItem = NULL;

    // Check if category already exists
    for(int cat = 0; cat < topLevelItemCount(); cat++)
    {
        if(topLevelItem(cat)->text(0) == catStr)
        {
            catItem = (EntityDatumItem*)(topLevelItem(cat));
            break;
        }
    }

    // If not found, create category
    if(catItem == NULL)
    {
        catItem = new EntityDatumItem(NULL, catStr);
        addTopLevelItem(catItem);
        // Check filter
        if(!catStr.contains(active_filter, Qt::CaseInsensitive))
            catItem->setHidden(true);
    }

    EntityDatumItem* newItem = new EntityDatumItem(catItem, datum);
    if(catItem->isExpanded())
        newItem->activate(this);
    else
        catItem->setExpanded(true);

    // Check filter
    if(newItem->getName().contains(active_filter, Qt::CaseInsensitive) ||
       catItem->getName().contains(active_filter, Qt::CaseInsensitive))
    {
        newItem->setHidden(false);
        catItem->setHidden(false);
    }
    else
        newItem->setHidden(true);
}

void EntityDataList::activateItem(QModelIndex idx)
{
    EntityDatumItem* dItem = (EntityDatumItem*)(this->itemFromIndex(idx));
    if(dItem != NULL)
        dItem->activate(this);
    else
        std::cerr << "EntityDataList::activateItem ERROR: " << idx.row() << " does not exist!" << std::endl;
}

void EntityDataList::deactivateItem(QModelIndex idx)
{
    EntityDatumItem* dItem = (EntityDatumItem*)(this->itemFromIndex(idx));
    if(dItem != NULL)
        dItem->deactivate(this);
    else
        std::cerr << "EntityDataList::deactivateItem ERROR: " << idx.row() << " does not exist!" << std::endl;
}

void EntityDataList::setActiveEntity(std::string entity)
{
    mutex.lock();

    if(active_entity != entity)
    {
        clear();
        active_entity = entity;

        if(datum_map.count(entity))
        {
            std::vector<const DatumInfo*>* datums = datum_map[entity];
            std::vector<const DatumInfo*>::const_iterator it;
            for(it = datums->begin(); it != datums->end(); it++)
            {
                const DatumInfo* datum = *it;
                addItem(datum);
            }
        }
    }

    mutex.unlock();
}

// Show/hide items based on whether or not they
// match the passed in string
void EntityDataList::filterList(QString str)
{
    active_filter = str;
    for(int idx=0; idx < topLevelItemCount(); idx++)
    {
        bool showCategory = false;
        EntityDatumItem* item = (EntityDatumItem*)(topLevelItem(idx));
        if(item->getName().contains(active_filter, Qt::CaseInsensitive))
        {
            showCategory = true;
            // For category match, show all children
            for(int kidIdx = 0; kidIdx < item->childCount(); kidIdx++)
                item->child(kidIdx)->setHidden(false);
        }
        else // Check children for a match
        {
            for(int kidIdx = 0; kidIdx < item->childCount(); kidIdx++)
            {
                EntityDatumItem* kid = (EntityDatumItem*)(item->child(kidIdx));
                if(kid->getName().contains(active_filter, Qt::CaseInsensitive))
                {
                    showCategory = true;
                    kid->setHidden(false);
                }
                else
                    kid->setHidden(true);
            }
        }

        if(showCategory)
            item->setHidden(false);
        else
            item->setHidden(true);
    }
}

QString EntityDataList::getActiveFilter()
{
    return active_filter;
}

void EntityDataList::notifyNewDatum(const DatumInfo *datum)
{
    mutex.lock();

    std::vector<const DatumInfo*>* datums;

    std::string entity = datum->getEntityName();
    if(datum_map.count(entity))
    {
        datums = datum_map[entity];
    }
    else
    {
        datums = new std::vector<const DatumInfo*>();
        datum_map[entity] = datums;
    }

    datums->push_back(datum);

    if(entity == active_entity)
        addItem(datum);

    mutex.unlock();
}

void EntityDataList::notifyNewValue(const DatumInfo* datum)
{
    std::string entity = datum->getEntityName();
    std::cerr << "This should not get called:" << entity << std::endl;
}

void EntityDataList::notifyEntityRemoved(std::string entity)
{
    static std::string lastEntityRemoved;
    lastEntityRemoved = entity;
}

void EntityDataList::notifyAllDatumsInvalid()
{
    mutex.lock();

    clear();
    std::map<std::string, std::vector<const DatumInfo*>* >::iterator it;
    for(it = datum_map.begin(); it != datum_map.end(); it++)
    {
        std::vector<const DatumInfo*>* vecPtr = it->second;
        delete vecPtr;
    }
    datum_map.clear();

    mutex.unlock();
}

QString EntityDataList::getStringRepresentation() const
{
    QString rep("<EntityDataList>\n");
    rep += "<Filter>" + active_filter + "</Filter>\n";
    rep += "</EntityDataList>\n";

    return rep;
}

EntityDataList* EntityDataList::createFromStringRepresentation(QString rep,
                                                           QWidget* parent)
{
    EntityDataList* ret = new EntityDataList(parent);
    std::string guts = Configuration::getTagValue(rep.toStdString(), "EntityDataList");
    ret->active_filter = QString(Configuration::getTagValue(guts, "Filter").c_str());
    return ret;
}





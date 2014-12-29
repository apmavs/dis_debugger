#include "EntityDataList.h"


EntityDataList::EntityDataList(QWidget *parent) :
    QTreeWidget(parent)
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
    DatumItem* catItem = NULL;

    // Check if category already exists
    for(int cat = 0; cat < topLevelItemCount(); cat++)
    {
        if(topLevelItem(cat)->text(0) == catStr)
        {
            catItem = (DatumItem*)(topLevelItem(cat));
            break;
        }
    }

    // If not found, create category
    if(catItem == NULL)
    {
        catItem = new DatumItem(catStr);
        addTopLevelItem(catItem);
    }

    DatumItem* newItem = new DatumItem(catItem, datum);
    if(catItem->isExpanded())
        newItem->activate(this);
    else
        catItem->setExpanded(true);
}

void EntityDataList::activateItem(QModelIndex idx)
{
    DatumItem* dItem = (DatumItem*)(this->itemFromIndex(idx));
    if(dItem != NULL)
        dItem->activate(this);
    else
        std::cerr << "EntityDataList::activateItem ERROR: " << idx.row() << " does not exist!" << std::endl;
}

void EntityDataList::deactivateItem(QModelIndex idx)
{
    DatumItem* dItem = (DatumItem*)(this->itemFromIndex(idx));
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

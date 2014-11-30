#include "EntityDataList.h"


EntityDataList::EntityDataList(QWidget *parent) :
    QListWidget(parent)
{
    controller = DataModelController::getInstance();
    controller->registerObserver(this);
    active_entity = "";
}

QString EntityDataList::getDisplayString(const DatumInfo* datum)
{
    std::string retVal = datum->getName() + ": " + datum->getValue();
    return QString(retVal.c_str());
}

void EntityDataList::setActiveEntity(std::string entity)
{
    mutex.lock();

    if(active_entity != entity)
    {
        clear();
        active_entity = entity;

        if(datum_map.count(active_entity))
        {
            std::vector<const DatumInfo*>* datums = datum_map[entity];
            std::vector<const DatumInfo*>::const_iterator it;
            for(it = datums->begin(); it != datums->end(); it++)
            {
                const DatumInfo* datum = *it;
                addItem(getDisplayString(datum));
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
    controller->registerDatumObserver(this, datum);

    if(entity == active_entity)
    {
        QString newVal = getDisplayString(datum);
        addItem(newVal);
    }

    mutex.unlock();
}

void EntityDataList::notifyNewValue(const DatumInfo* datum)
{
    mutex.lock();

    std::string entity = datum->getEntityName();
    if(datum_map.count(entity))
    {
        std::vector<const DatumInfo*>* datums = datum_map[entity];
        std::vector<const DatumInfo*>::const_iterator it;
        int pos = 0;
        for(it = datums->begin(); it != datums->end(); it++)
        {
            if(datum->getId() == (*it)->getId())
            {
                if(entity == active_entity)
                {
                    QString newVal = getDisplayString(datum);
                    item(pos)->setText(newVal);
                }
                (*datums)[pos] = datum;

                break;
            }
            pos++;
        }
    }

    mutex.unlock();
}

void EntityDataList::notifyEntityRemoved(std::string entity)
{
    static std::string lastEntityRemoved;
    lastEntityRemoved = entity;
}

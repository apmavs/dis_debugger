#include "EntityDataList.h"


EntityDataList::EntityDataList(QWidget *parent) :
    QTreeWidget(parent)
{
    controller = DataModelController::getInstance();
    controller->registerObserver(this);
    active_entity = "";
}

void EntityDataList::addItem(const DatumInfo* datum)
{
    QString catStr = QString(datum->getCategory().c_str());
    QTreeWidgetItem* catItem = NULL;

    // Check if category already exists
    for(int cat = 0; cat < topLevelItemCount(); cat++)
    {
        if(topLevelItem(cat)->text(0) == catStr)
        {
            catItem = topLevelItem(cat);
            break;
        }
    }

    // If not found, create category
    if(catItem == NULL)
    {
        catItem = new QTreeWidgetItem(QStringList(catStr));
        addTopLevelItem(catItem);
    }

    QStringList strings = QStringList(datum->getName().c_str());
    std::string val = datum->getValue() + " " + datum->getUnit();
    strings.append(val.c_str());
    QTreeWidgetItem* newItem = new QTreeWidgetItem(catItem, strings);
    catItem->setExpanded(true);
    newItem->setToolTip(0, QString(datum->getDescription().c_str()));
    newItem->setToolTip(1, QString(datum->getDescription().c_str()));
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
    controller->registerDatumObserver(this, datum);

    if(entity == active_entity)
    {
        addItem(datum);
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
                    QString catStr(datum->getCategory().c_str());
                    QString nameStr(datum->getName().c_str());
                    for(int catIdx = 0; catIdx < topLevelItemCount(); catIdx++)
                    {
                        QTreeWidgetItem* catItem = topLevelItem(catIdx);
                        if(catItem->text(0) == catStr)
                        {
                            for(int kidIdx = 0;
                                kidIdx < catItem->childCount();
                                kidIdx++)
                            {
                                QTreeWidgetItem* kidItem =
                                        catItem->child(kidIdx);
                                if(kidItem->text(0) == nameStr)
                                {
                                    std::string val = datum->getValue();
                                    val += " " + datum->getUnit();
                                    kidItem->setText(1, QString(val.c_str()));
                                    break;
                                }
                            }
                            break;
                        }
                    }
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

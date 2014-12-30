#ifndef ENTITYDATALIST_H
#define ENTITYDATALIST_H

#include "DatumObserver.h"
#include "DataModelController.h"
#include "EntityDatumItem.h"

#include <string>
#include <map>
#include <QString>
#include <QMutex>
#include <QTreeWidget>

class EntityDataList : public QTreeWidget, DatumObserver
{
    Q_OBJECT
protected:
    QMutex mutex;
    // Map entity names to associated datums
    std::map<std::string, std::vector<const DatumInfo*>* > datum_map;
    DataModelController* controller;
    std::string active_entity;

    void addItem(const DatumInfo* datum);

protected slots:
    void activateItem(QModelIndex idx);
    void deactivateItem(QModelIndex idx);

public:
    explicit EntityDataList(QWidget *parent = 0);

    void setActiveEntity(std::string entity);

    // DatumObserver inheritance
    virtual void notifyNewDatum(const DatumInfo* datum);
    virtual void notifyNewValue(const DatumInfo* datum);
    virtual void notifyEntityRemoved(std::string entity);
    virtual void notifyAllDatumsInvalid();

};

#endif // ENTITYDATALIST_H

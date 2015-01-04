#ifndef DATUMITEM_H
#define DATUMITEM_H

#include "DatumInfo.h"
#include "DataModelController.h"

#include <set>
#include <QMutex>
#include <QObject>

class DatumItem : public QObject, public DatumObserver
{
    Q_OBJECT

private:
    DatumItem();
    DatumItem(const DatumItem& rhs);
    DatumItem& operator=(const DatumItem& rhs);

private slots:
    // These are slots to ensure GUI updates are done in GUI thread
    virtual void setDisplay() = 0;
    virtual void clearDisplay() = 0;

signals:
    void updateDisplay();
    void stopUpdates();

protected:
    QMutex mutex;
    DataModelController* controller;
    std::set<const void*> interested_widgets;
    const DatumInfo* const watched_datum;
    QString category_name;
    std::string entity_removed;
    const DatumInfo* new_datum;
    bool constant_updates;

    DatumItem(QString categoryName); // For category items with no datum
    DatumItem(const DatumInfo* d, bool constantUpdates = false);

public:
    virtual ~DatumItem();

    const DatumInfo* getWatchedDatum();
    QString getCategoryName();

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

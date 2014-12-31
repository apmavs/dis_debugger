#ifndef WATCHLIST_H
#define WATCHLIST_H

#include "EntityDatumItem.h"

#include <QTreeWidget>
#include <QDropEvent>

class WatchList : public QTreeWidget
{
    Q_OBJECT

protected:
    virtual void dropEvent(QDropEvent* event);

protected slots:
    void activateItem(QModelIndex idx);
    void deactivateItem(QModelIndex idx);

public:
    explicit WatchList(QWidget *parent = 0);
};

#endif // WATCHLIST_H

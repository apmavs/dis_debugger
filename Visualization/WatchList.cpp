#include "WatchList.h"
#include "EntityDataList.h"
#include "WatchDatumItem.h"

WatchList::WatchList(QWidget *parent) :
    QTreeWidget(parent)
{
    connect(this, SIGNAL(expanded(QModelIndex)), this, SLOT(activateItem(QModelIndex)));
    connect(this, SIGNAL(collapsed(QModelIndex)), this, SLOT(deactivateItem(QModelIndex)));
}

void WatchList::dropEvent(QDropEvent* event)
{
    EntityDataList* list = dynamic_cast<EntityDataList*>(event->source());
    if(list != NULL)
    {
        QList<QTreeWidgetItem*> items = list->selectedItems();
        for(int itemNum = 0; itemNum < items.size(); itemNum++)
        {
            EntityDatumItem* item = dynamic_cast<EntityDatumItem*>(items.at(itemNum));
            if(item != NULL)
            {
                WatchDatumItem* watchItem = WatchDatumItem::createWatchItem(item);
                addTopLevelItem(watchItem);
                if(watchItem->isExpanded())
                    watchItem->activate(this);
                else
                    watchItem->setExpanded(true);
            }
        }
    }
    event->setDropAction(Qt::IgnoreAction);
    event->accept();
    QTreeView::dropEvent(event);
}

void WatchList::activateItem(QModelIndex idx)
{
    EntityDatumItem* dItem = (EntityDatumItem*)(this->itemFromIndex(idx));
    if(dItem != NULL)
        dItem->activate(this);
    else
        std::cerr << "EntityDataList::activateItem ERROR: " << idx.row() << " does not exist!" << std::endl;
}

void WatchList::deactivateItem(QModelIndex idx)
{
    EntityDatumItem* dItem = (EntityDatumItem*)(this->itemFromIndex(idx));
    if(dItem != NULL)
        dItem->deactivate(this);
    else
        std::cerr << "EntityDataList::deactivateItem ERROR: " << idx.row() << " does not exist!" << std::endl;
}



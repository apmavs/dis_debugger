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
    QModelIndex index = indexAt(event->pos());
    EntityDataList* entityList = dynamic_cast<EntityDataList*>(event->source());
    WatchList* watchList = dynamic_cast<WatchList*>(event->source());
    if(entityList != NULL)
    {
        QList<QTreeWidgetItem*> items = entityList->selectedItems();
        addItems(items, index);
    }
    else if(watchList != NULL)
    {
        QList<QTreeWidgetItem*> items = watchList->selectedItems();
        moveItems(items, index);
    }

    event->setDropAction(Qt::IgnoreAction);
    event->accept();
    QTreeView::dropEvent(event);
}

void WatchList::addItems(QList<QTreeWidgetItem*> items, QModelIndex toIdx)
{
    QList<QTreeWidgetItem*> newItems;
    for(int itemNum = 0; itemNum < items.size(); itemNum++)
    {   // Create items and add to list
        EntityDatumItem* item = dynamic_cast<EntityDatumItem*>(items.at(itemNum));
        if(item != NULL)
        {
            WatchDatumItem* watchItem = WatchDatumItem::createWatchItem(item);
            newItems.append(watchItem);
            addTopLevelItem(watchItem);
        }
    }

    // Call move items on all created items to move them to dropped spot
    if(newItems.size() > 0)
        moveItems(newItems, toIdx);

    // Initially add so they are expanded
    for(int itemNum = 0; itemNum < newItems.size(); itemNum++)
    {
        QTreeWidgetItem* newItem = newItems.at(itemNum);
        newItem->setExpanded(true);
    }
}

void WatchList::moveItems(QList<QTreeWidgetItem*> items, QModelIndex toIdx)
{
    if(!toIdx.isValid())
        return;

    for(int itemNum = 0; itemNum < items.size(); itemNum++)
    {
        WatchDatumItem* dropItem = dynamic_cast<WatchDatumItem*>(items.at(itemNum));
        if(dropItem != NULL)
        {
            QTreeWidgetItem* destItem = itemFromIndex(toIdx);
            if(destItem != NULL)
            {
                int dropIdx, destIdx;

                QTreeWidgetItem* dropParent = ((QTreeWidgetItem*)(dropItem))->parent();
                if(dropParent != NULL)
                {
                    dropIdx = dropParent->indexOfChild(dropItem);
                    dropParent->takeChild(dropIdx); // Remove to move
                }
                else
                {
                    dropIdx = dropItem->treeWidget()->indexOfTopLevelItem(dropItem);
                    dropItem->treeWidget()->takeTopLevelItem(dropIdx); // Remove to move
                }

                QTreeWidgetItem* destParent = destItem->parent();
                if(destParent != NULL)
                    destIdx = destParent->indexOfChild(destItem);
                else
                    destIdx = destItem->treeWidget()->indexOfTopLevelItem(destItem);

                DropIndicatorPosition pos = dropIndicatorPosition();
                if(pos == QAbstractItemView::OnItem)
                    destItem->addChild(dropItem);
                else if(pos == QAbstractItemView::AboveItem)
                {
                    if(destParent != NULL) destParent->insertChild(destIdx, dropItem);
                    else destItem->treeWidget()->insertTopLevelItem(destIdx, dropItem);
                }
                else // BelowItem
                {
                    if(destParent != NULL) destParent->insertChild(destIdx+1, dropItem);
                    else destItem->treeWidget()->insertTopLevelItem(destIdx+1, dropItem);
                }
            }
        }
    }
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



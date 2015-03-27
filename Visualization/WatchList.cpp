#include "WatchList.h"
#include "EntityDataList.h"
#include "WatchDatumItem.h"

WatchList::WatchList(QWidget *parent) :
    QTreeWidget(parent),
    right_click_menu(tr(""), this),
    delete_action("Delete", this)
{
    // Create right click menu
    connect(&delete_action, SIGNAL(triggered()), this, SLOT(deleteSelectedItems()));
    right_click_menu.addAction(&delete_action);

    // Setup signal to show the right click menu
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showRightClickMenu(QPoint)));

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

void WatchList::keyPressEvent(QKeyEvent* event)
{
    if(event->key() == Qt::Key_Delete)
        deleteSelectedItems();
    else
        QWidget::keyPressEvent(event);
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

void WatchList::deleteSelectedItems()
{
    // Continuously grab all selected items after each single delete to
    // prevent calling delete on children multiple times if the user
    // selected both child and it's parent for the delete.
    QList<QTreeWidgetItem*> items = selectedItems();
    while(items.size() > 0)
    {
        QTreeWidgetItem* item = items.at(0);
        QTreeWidgetItem* itemParent = item->parent();
        if(itemParent != NULL)
        {
            int itemIdx = itemParent->indexOfChild(item);
            item = itemParent->takeChild(itemIdx);
        }
        else
        {
            int itemIdx = item->treeWidget()->indexOfTopLevelItem(item);
            item = item->treeWidget()->takeTopLevelItem(itemIdx);
        }
        delete item;
        items = selectedItems();
    }
}

void WatchList::showRightClickMenu(const QPoint& pos)
{
    right_click_menu.exec(mapToGlobal(pos));
}

bool WatchList::equivalentTo(WatchList* rhs)
{
    bool ret = true;

    if(topLevelItemCount() != rhs->topLevelItemCount())
        ret = false;
    else
    {
        for(int idx = 0; idx < topLevelItemCount(); idx++)
        {
            WatchDatumItem* myItem  = (WatchDatumItem*)(topLevelItem(idx));
            WatchDatumItem* rhsItem = (WatchDatumItem*)(rhs->topLevelItem(idx));
            if(!(myItem->equivalentTo(rhsItem)))
            {
                ret = false;
                break;
            }
        }
    }

    return ret;
}

QString WatchList::getStringRepresentation() const
{
    QString rep = "<WatchList>\n";

    for(int idx = 0; idx < topLevelItemCount(); idx++)
    {
        WatchDatumItem* item = (WatchDatumItem*)(topLevelItem(idx));
        rep += "<WatchItem>\n";
        rep += item->getStringRepresentation();
        rep += "</WatchItem>\n";
    }

    rep += "</WatchList>\n";

    return rep;
}

void WatchList::setFromStringRepresentation(QString rep)
{
    // Clear out old items and replace with items from string
    clear();

    QString guts = QString(Configuration::getTagValue(rep.toStdString(), "WatchList").c_str());

    // Get all watch items
    QString endTag = "</WatchItem>";
    QString watchData = QString(Configuration::getTagValue(guts.toStdString(),
                                                         "WatchItem").c_str());
    QList<QTreeWidgetItem*> items;
    while(watchData != "")
    {
        WatchDatumItem* watchItem = WatchDatumItem::
                createFromStringRepresentation(watchData, NULL);
        items.append(watchItem);

        int endTagPos = guts.indexOf(endTag);
        if(endTagPos >= 0)
        {
            guts = guts.remove(0, endTagPos + endTag.length());
            watchData = QString(Configuration::getTagValue(guts.toStdString(),
                                                         "WatchItem").c_str());
        }
        else
        {
            std::cerr << __FILE__ << ": Broken XML:" + rep.toStdString() << std::endl;
            break;
        }
    }

    if(!items.empty())
        addItems(items, QModelIndex());
}



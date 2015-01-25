#ifndef WATCHLIST_H
#define WATCHLIST_H

#include "EntityDatumItem.h"

#include <QTreeWidget>
#include <QDropEvent>
#include <QMenu>

class WatchList : public QTreeWidget
{
    Q_OBJECT

protected:
    QMenu right_click_menu;
    QAction delete_action;

    virtual void dropEvent(QDropEvent* event);
    void addItems(QList<QTreeWidgetItem*> items, QModelIndex toIdx);
    void moveItems(QList<QTreeWidgetItem*> items, QModelIndex toIdx);
    void keyPressEvent(QKeyEvent* event);

protected slots:
    void activateItem(QModelIndex idx);
    void deactivateItem(QModelIndex idx);
    void deleteSelectedItems();
    void showRightClickMenu(const QPoint& pos);

public:
    explicit WatchList(QWidget *parent = 0);
};

#endif // WATCHLIST_H

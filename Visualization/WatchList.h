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
    void moveItems(QList<QTreeWidgetItem*> items, QModelIndex toIdx);
    void keyPressEvent(QKeyEvent* event);

protected slots:
    void activateItem(QModelIndex idx);
    void deactivateItem(QModelIndex idx);
    void deleteSelectedItems();
    void showRightClickMenu(const QPoint& pos);

public:
    explicit WatchList(QWidget *parent = 0);
    void addItems(QList<QTreeWidgetItem*> items, QModelIndex toIdx);

    bool equivalentTo(WatchList* rhs);
    virtual QString getStringRepresentation() const;
    static WatchList* createFromStringRepresentation(QString rep,
                                          QWidget* parent = NULL);
};

#endif // WATCHLIST_H

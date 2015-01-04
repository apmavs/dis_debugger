#include "PlotGroupBox.h"
#include "PlotWidget.h"
#include <iostream>
#include <QTreeWidget>

PlotGroupBox::PlotGroupBox(QWidget* parent) : QGroupBox(parent)
{

}

PlotGroupBox::~PlotGroupBox()
{

}

void PlotGroupBox::addCurveToPlots(const DatumInfo* datum)
{
    if(datum != NULL)
    {
        QList<PlotWidget*> plotWidgets = findChildren<PlotWidget*>();
        for(int i = 0; i < plotWidgets.size(); i++)
        {
            PlotWidget* plotWidget = plotWidgets.at(i);
            plotWidget->addCurve(datum);
        }
    }
}

void PlotGroupBox::dragEnterEvent(QDragEnterEvent* event)
{
    // Only accept if item being dropped contains a DatumInfo
    bool accept = false;
    QTreeWidget* list = dynamic_cast<QTreeWidget*>(event->source());
    if(list != NULL)
    {
        QList<QTreeWidgetItem*> items = list->selectedItems();
        for(int itemNum = 0; itemNum < items.size(); itemNum++)
        {
            EntityDatumItem* item = dynamic_cast<EntityDatumItem*>(items.at(itemNum));
            if(item != NULL)
            {
                accept = true;
                break;
            }
        }
    }

    if(accept)
        event->acceptProposedAction();
}

void PlotGroupBox::dropEvent(QDropEvent* event)
{
    QTreeWidget* list = dynamic_cast<QTreeWidget*>(event->source());
    if(list != NULL)
    {
        QList<QTreeWidgetItem*> items = list->selectedItems();
        for(int itemNum = 0; itemNum < items.size(); itemNum++)
        {
            EntityDatumItem* item = dynamic_cast<EntityDatumItem*>(items.at(itemNum));
            if(item != NULL)
            {
                addCurveToPlots(item->getWatchedDatum());
                // Also add any children
                for(int kidIdx = 0; kidIdx < item->childCount(); kidIdx++)
                {
                    EntityDatumItem* kid =
                            dynamic_cast<EntityDatumItem*>(item->child(kidIdx));
                    if(kid != NULL)
                        addCurveToPlots(kid->getWatchedDatum());
                }
            }
        }
    }

    event->setDropAction(Qt::IgnoreAction);
    event->accept();
    QGroupBox::dropEvent(event);
}

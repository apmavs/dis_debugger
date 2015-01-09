#include "PlotGroupBox.h"
#include "PlotWidget.h"
#include <iostream>
#include <QTreeWidget>
#include <QLayout>

PlotGroupBox::PlotGroupBox(QWidget* parent) : QGroupBox(parent)
{
    drop_box_label = NULL;
}

PlotGroupBox::~PlotGroupBox()
{

}

void PlotGroupBox::setPlotGroup(QGroupBox* g)
{
    plot_group = g;
}

void PlotGroupBox::setDropBoxLabel(QLabel* l)
{
    drop_box_label = l;
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
    if((drop_box_label != NULL) && (plot_group != NULL) &&
            drop_box_label->underMouse() &&
            (list != NULL))
    {
        PlotWidget* newPlot = new PlotWidget(this);
        plot_group->layout()->addWidget(newPlot);

        QList<QTreeWidgetItem*> items = list->selectedItems();
        for(int itemNum = 0; itemNum < items.size(); itemNum++)
        {
            EntityDatumItem* item = dynamic_cast<EntityDatumItem*>(items.at(itemNum));
            if(item != NULL)
            {
                newPlot->addCurve(item->getWatchedDatum());
                // Also add any children
                for(int kidIdx = 0; kidIdx < item->childCount(); kidIdx++)
                {
                    EntityDatumItem* kid =
                            dynamic_cast<EntityDatumItem*>(item->child(kidIdx));
                    if(kid != NULL)
                        newPlot->addCurve(kid->getWatchedDatum());
                }
            }
        }
    }

    event->setDropAction(Qt::IgnoreAction);
    event->accept();
    QGroupBox::dropEvent(event);
}

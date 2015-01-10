#include "PlotGroupBox.h"
#include "ui_PlotGroupBox.h"
#include <iostream>
#include <QTreeWidget>
#include <QLayout>

PlotGroupBox::PlotGroupBox(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::PlotGroupBox)
{
    ui->setupUi(this);
}

PlotGroupBox::~PlotGroupBox()
{

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
        PlotWidget* curPlot = NULL;

        // Check if user is trying to create a new plot
        if(ui->PlotDropLabel->underMouse())
        {
            curPlot = new PlotWidget(this);
            created_plots.append(curPlot);
            ui->PlotScrollContents->layout()->addWidget(curPlot);
        }
        else // Check if user is adding to existing plot
        {
            if(ui->FirstPlot->underMouse())
                curPlot = ui->FirstPlot;
            else
            {
                for(int plotIdx = 0; plotIdx < created_plots.size(); plotIdx++)
                {
                    PlotWidget* p = created_plots.at(plotIdx);
                    if(p->underMouse())
                    {
                        curPlot = p;
                        break;
                    }
                }
            }
        }

        // If dropped over a plot or created a new plot, add curves to it
        if(curPlot != NULL)
        {
            QList<QTreeWidgetItem*> items = list->selectedItems();
            curPlot->addCurves(items);
        }
    }

    // Change event to ignore and accept to make QT happy
    event->setDropAction(Qt::IgnoreAction);
    event->accept();
    QWidget::dropEvent(event);
}

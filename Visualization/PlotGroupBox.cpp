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
    // Re-create the initial plot, QT auto generate screws up the style sheet
    PlotWidget* permPlot = new PlotWidget(this);
    PlotWidget* oldPlot = ui->FirstPlot;
    ui->FirstPlot = permPlot;
    ui->PlotSplitter->addWidget(permPlot);
    oldPlot->setParent(NULL);
    delete oldPlot;
    ui->FirstPlot->hideDelete(); // Always keep 1 plot, don't allow delete btn
    ui->HideMeBtn->hide();
}

PlotGroupBox::~PlotGroupBox()
{

}

void PlotGroupBox::on_DeleteAllBtn_clicked()
{
    deleteAllPlots();
}

void PlotGroupBox::deletePlot(PlotWidget* plot)
{
    for(int plotIdx = 0; plotIdx < created_plots.size(); plotIdx++)
    {
        PlotWidget* p = created_plots.at(plotIdx);
        if(p == plot)
        {
            created_plots.removeAt(plotIdx);
            plot->setParent(NULL);
            delete plot;
            break;
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
        PlotWidget* curPlot = NULL;

        // Check if user is trying to create a new plot
        if(ui->PlotDropLabel->rect().contains(
                    ui->PlotDropLabel->mapFromGlobal(QCursor::pos())))
        {
            curPlot = new PlotWidget(this);
            connect(curPlot, SIGNAL(deletePlot(PlotWidget*)),
                    this, SLOT(deletePlot(PlotWidget*)));
            created_plots.append(curPlot);
            ui->PlotSplitter->addWidget(curPlot);
        }
        else // Check if user is adding to existing plot
        {
            if(ui->FirstPlot->rect().contains(
               ui->FirstPlot->mapFromGlobal(QCursor::pos())))
            {
                curPlot = ui->FirstPlot;
            }
            else
            {
                for(int plotIdx = 0; plotIdx < created_plots.size(); plotIdx++)
                {
                    PlotWidget* p = created_plots.at(plotIdx);
                    if(p->underMouse())
                    if(p->rect().contains(
                       p->mapFromGlobal(QCursor::pos())))
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

void PlotGroupBox::deleteAllPlots()
{
    // Clear permanent plot
    ui->FirstPlot->clearCurves();

    // Delete created plots
    while(created_plots.size())
    {
        PlotWidget* p = created_plots.takeAt(0);
        p->setParent(NULL);
        delete p;
    }
}

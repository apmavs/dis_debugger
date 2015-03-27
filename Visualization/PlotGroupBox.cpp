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
            addPlot(curPlot);
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

void PlotGroupBox::addPlot(PlotWidget* plot)
{
    connect(plot, SIGNAL(deletePlot(PlotWidget*)),
            this, SLOT(deletePlot(PlotWidget*)));
    created_plots.append(plot);
    ui->PlotSplitter->addWidget(plot);
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

void PlotGroupBox::addToFirstPlot(QList<QTreeWidgetItem*> items)
{
    ui->FirstPlot->addCurves(items);
}

bool PlotGroupBox::equivalentTo(PlotGroupBox* rhs)
{
    bool ret = true;

    if(!(ui->FirstPlot->equivalentTo(rhs->ui->FirstPlot)))
        ret = false;
    else if(created_plots.length() != rhs->created_plots.length())
        ret = false;
    else
    {
        for(int idx = 0; idx < created_plots.length(); idx++)
        {
            PlotWidget* myPlot  = created_plots.at(idx);
            PlotWidget* rhsPlot = rhs->created_plots.at(idx);
            if(!(myPlot->equivalentTo(rhsPlot)))
            {
                ret = false;
                break;
            }
        }
    }

    return ret;
}

QString PlotGroupBox::getStringRepresentation() const
{
    QString rep = "<PlotGroupBox>\n";

    rep += "<FirstPlot>\n";
    rep += ui->FirstPlot->getStringRepresentation();
    rep += "</FirstPlot>\n";

    for(int idx = 0; idx < created_plots.length(); idx++)
    {
        PlotWidget* p = created_plots.at(idx);
        rep += "<Plot>\n";
        rep += p->getStringRepresentation();
        rep += "</Plot>\n";
    }

    rep += "</PlotGroupBox>\n";

    return rep;
}

void PlotGroupBox::setFromStringRepresentation(QString rep)
{
    // Delete any previous plots and create new ones from string
    deleteAllPlots();

    QString guts = QString(Configuration::getTagValue(rep.toStdString(),
                                                      "PlotGroupBox").c_str());

    QString firstStr = QString(Configuration::getTagValue(guts.toStdString(),
                                                          "FirstPlot").c_str());
    ui->FirstPlot->setFromStringRepresentation(firstStr);

    // Get all plots
    QString endTag = "</Plot>";
    QString plotData = QString(Configuration::getTagValue(guts.toStdString(),
                                                             "Plot").c_str());
    while(plotData != "")
    {
        PlotWidget* p = PlotWidget::createFromStringRepresentation(plotData);
        addPlot(p);

        int endTagPos = guts.indexOf(endTag);
        if(endTagPos >= 0)
        {
            guts = guts.remove(0, endTagPos + endTag.length());
            plotData = QString(Configuration::getTagValue(guts.toStdString(),
                                                         "Plot").c_str());
        }
        else
        {
            std::cerr << __FILE__ << ": Broken XML:" + rep.toStdString() << std::endl;
            break;
        }
    }
}

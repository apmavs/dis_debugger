#include "PlotWidget.h"
#include "ui_PlotWidget.h"
#include <qwt_plot_layout.h>
#include <qwt_legend.h>

const QColor PlotWidget::COLOR_WHEEL[] = {Qt::black,
                                          Qt::red,
                                          Qt::blue,
                                          Qt::green,
                                          Qt::magenta,
                                          Qt::cyan,
                                          Qt::yellow,
                                          Qt::gray,
                                          Qt::darkRed,
                                          Qt::darkBlue,
                                          Qt::darkGreen,
                                          Qt::darkMagenta,
                                          Qt::darkCyan,
                                          Qt::darkYellow,
                                          Qt::darkGray};

PlotWidget::PlotWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PlotWidget),
    color_index(0)
{
    ui->setupUi(this);
    QString toolTip = "Select area to zoom\n";
    toolTip += "Press 'U' to undo zoom\n";
    toolTip += "Press 'R' to redo zoom\n";
    toolTip += "Press HOME key to return to autoscale";
    setToolTip(toolTip);

    // Put a legend on plot
    QwtLegend* legend = new QwtLegend();
    legend->setFrameStyle(QFrame::Box | QFrame::Sunken);
    ui->embedded_plot->insertLegend(legend, QwtPlot::BottomLegend);

    zoomer = new QwtPlotZoomer(ui->embedded_plot->canvas());
    curves.clear();

    // Setup zoomer
    zoomer->setMousePattern(QwtEventPattern::MouseSelect1, Qt::LeftButton);
    zoomer->setKeyPattern(QwtEventPattern::KeyRedo, Qt::Key_R);
    zoomer->setKeyPattern(QwtEventPattern::KeyUndo, Qt::Key_U);
    zoomer->setKeyPattern(QwtEventPattern::KeyHome, Qt::Key_Home );
    connect(zoomer, SIGNAL(zoomed(QRectF)), this, SLOT(zoomChanged(QRectF)));
}

PlotWidget::~PlotWidget()
{
    clearCurves();
    delete zoomer;
    delete ui;
}

QColor PlotWidget::getNextColor()
{
    QColor retVal = COLOR_WHEEL[color_index];
    color_index++;
    size_t numColors = sizeof(COLOR_WHEEL) / sizeof(COLOR_WHEEL[0]);
    if(color_index >= numColors)
        color_index = 0;
    return retVal;
}

void PlotWidget::zoomChanged(const QRectF&)
{
    const QwtPlotZoomer* zoomer = dynamic_cast<const QwtPlotZoomer*>(sender());
    if(zoomer != NULL)
    {
        // If stack index is 0, then we cannot undo any more, so we're at the
        // initial state when autoscale should be on
        if(zoomer->zoomRectIndex() == 0)
        {
            ui->embedded_plot->setAxisAutoScale(zoomer->xAxis(), true);
            ui->embedded_plot->setAxisAutoScale(zoomer->yAxis(), true);
            ui->embedded_plot->replot();
        }
    }
}

void PlotWidget::addCurve(EntityDatumItem* item)
{
    if(item != NULL)
    {
        const DatumInfo* datum = item->getWatchedDatum();
        if(datum != NULL)
        {
            PlotCurveItem* curve = new PlotCurveItem(ui->embedded_plot,
                                                     datum,
                                                     getNextColor());
            curves.append(curve);
            curve->activate(this);
        }

        // Also add any children
        for(int kidIdx = 0; kidIdx < item->childCount(); kidIdx++)
        {
            EntityDatumItem* kid =
                    dynamic_cast<EntityDatumItem*>(item->child(kidIdx));
            addCurve(kid);
        }
    }
}

void PlotWidget::addCurves(QList<QTreeWidgetItem*> items)
{
    for(int itemNum = 0; itemNum < items.size(); itemNum++)
    {
        EntityDatumItem* item =
                dynamic_cast<EntityDatumItem*>(items.at(itemNum));
        addCurve(item);
    }
}

void PlotWidget::clearCurves()
{
    while(curves.size())
    {
        PlotCurveItem* i = curves.at(0);
        curves.removeAt(0);
        i->detach(); // Remove from plot
        delete i;
    }
    ui->embedded_plot->replot();
}

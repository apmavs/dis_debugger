#include "PlotWidget.h"
#include "ui_PlotWidget.h"

PlotWidget::PlotWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PlotWidget)
{
    ui->setupUi(this);
    QString toolTip = "Select area to zoom\n";
    toolTip += "Press 'U' to undo zoom\n";
    toolTip += "Press 'R' to redo zoom\n";
    toolTip += "Press HOME key to return to autoscale";
    setToolTip(toolTip);

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
    delete zoomer;
    delete ui->embedded_plot;
    delete ui;
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

void PlotWidget::addCurve(const DatumInfo* datum)
{
    if(datum != NULL)
    {
        PlotCurveItem* curve = new PlotCurveItem(ui->embedded_plot, datum);
        curve->activate(this);
    }
}

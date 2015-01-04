#include "PlotWidget.h"
#include "ui_PlotWidget.h"

PlotWidget::PlotWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PlotWidget)
{
    ui->setupUi(this);
    zoomer = new QwtPlotZoomer(ui->embedded_plot->canvas());
    curves.clear();

    // Setup zoomer
    zoomer->setMousePattern(QwtEventPattern::MouseSelect1, Qt::LeftButton);
    zoomer->setKeyPattern(QwtEventPattern::KeyRedo, Qt::Key_R, Qt::ControlModifier );
    zoomer->setKeyPattern(QwtEventPattern::KeyUndo, Qt::Key_U, Qt::ControlModifier );
    zoomer->setKeyPattern(QwtEventPattern::KeyHome, Qt::Key_Home );
}

PlotWidget::~PlotWidget()
{
    delete zoomer;
    delete ui->embedded_plot;
    delete ui;
}

void PlotWidget::addCurve(const DatumInfo* datum)
{
    if(datum != NULL)
    {
        PlotCurveItem* curve = new PlotCurveItem(ui->embedded_plot, datum);
        curve->activate(this);
    }
}

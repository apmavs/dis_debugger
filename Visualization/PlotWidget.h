#ifndef PLOTWIDGET_H
#define PLOTWIDGET_H

#include "PlotCurveItem.h"

#include <QWidget>
#include <qwt_plot.h>
#include <qwt_plot_zoomer.h>

namespace Ui {
class PlotWidget;
}

class PlotWidget : public QWidget
{
    Q_OBJECT
private:
    QwtPlotZoomer* zoomer;
    std::vector<PlotCurveItem*> curves;

public:
    explicit PlotWidget(QWidget *parent = 0);
    ~PlotWidget();

    void addCurve(const DatumInfo* datum);

private:
    Ui::PlotWidget *ui;
};

#endif // PLOTWIDGET_H

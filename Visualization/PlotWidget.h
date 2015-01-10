#ifndef PLOTWIDGET_H
#define PLOTWIDGET_H

#include "PlotCurveItem.h"
#include "EntityDatumItem.h"

#include <QWidget>
#include <qwt_plot.h>
#include <qwt_plot_zoomer.h>
#include <QRectF>
#include <QList>
#include <QTreeWidgetItem>

namespace Ui {
class PlotWidget;
}

class PlotWidget : public QWidget
{
    Q_OBJECT

private:
    Ui::PlotWidget* ui;
    QwtPlotZoomer* zoomer;
    std::vector<PlotCurveItem*> curves;

private slots:
    void zoomChanged(const QRectF&);

public:
    explicit PlotWidget(QWidget* parent = 0);
    ~PlotWidget();

    void addCurve(EntityDatumItem* item);
    void addCurves(QList<QTreeWidgetItem*> items);
};

#endif // PLOTWIDGET_H

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
#include <QColor>

namespace Ui {
class PlotWidget;
}

class PlotWidget : public QWidget
{
    Q_OBJECT

private:
    Ui::PlotWidget* ui;
    QwtPlotZoomer* zoomer;
    QList<PlotCurveItem*> curves;
    unsigned int color_index;
    static const QColor COLOR_WHEEL[];
    QColor getNextColor();

private slots:
    void zoomChanged(const QRectF&);
    void on_ClearBtn_clicked();
    void on_DeleteBtn_clicked();

signals:
    void deletePlot(PlotWidget* plot);

public:
    explicit PlotWidget(QWidget* parent = 0);
    ~PlotWidget();

    void hideDelete();
    void addCurve(EntityDatumItem* item);
    void addCurves(QList<QTreeWidgetItem*> items);
    void clearCurves();
};

#endif // PLOTWIDGET_H

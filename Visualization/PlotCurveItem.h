#ifndef PLOTCURVEITEM_H
#define PLOTCURVEITEM_H

#include "DatumItem.h"
#include "EntityDatumItem.h"
#include "DatumInfo.h"

#include <iostream>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <QVector>
#include <QPointF>


class PlotCurveItem : public QwtPlotCurve, public DatumItem
{
private:
    PlotCurveItem();
    PlotCurveItem(const DatumItem& rhs);
    PlotCurveItem& operator=(const DatumItem& rhs);
    QVector<QPointF> datum_points;
    QwtPlotCurve* curve_dots;

private slots:
    // These are slots to ensure GUI updates are done in GUI thread
    virtual void setDisplay();
    virtual void clearDisplay();

protected:
    static double convertToDouble(std::string s);

public:
    PlotCurveItem(QwtPlot* plot, const DatumInfo* d, QColor color);
    virtual ~PlotCurveItem();

    virtual void notifyNewValue(const DatumInfo* datum);
    void showFullHistory();
};

#endif // PLOTCURVEITEM_H

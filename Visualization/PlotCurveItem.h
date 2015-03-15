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
    QVector<QPointF> bad_points; // Track values out of min/max range
    QwtPlotCurve* curve_dots;
    QwtPlotCurve* bad_value_dots;
    QColor color;

private slots:
    // These are slots to ensure GUI updates are done in GUI thread
    virtual void setDisplay();
    virtual void clearDisplay();

protected:
    static double convertToDouble(std::string s);

public:
    PlotCurveItem(QwtPlot* plot, const DatumInfo* d, QColor c);
    virtual ~PlotCurveItem();

    virtual void notifyNewValue(const DatumInfo* datum);
    void showFullHistory();
    void truncateHistory();

    virtual bool equivalentTo(const PlotCurveItem* rhs) const;
    virtual QString getStringRepresentation() const;
    static PlotCurveItem* createFromStringRepresentation(QString rep,
                                                     QwtPlot* p);
};

#endif // PLOTCURVEITEM_H

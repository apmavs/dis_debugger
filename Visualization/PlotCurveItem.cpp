#include "PlotCurveItem.h"
#include <QByteArray>

PlotCurveItem::PlotCurveItem(QwtPlot* plot, const DatumInfo* d)
    : QwtPlotCurve(), DatumItem(d, true)
{
    double time   = d->getLastTimestamp();
    double curVal = convertToDouble(d->getValue());
    datum_points.append(QPointF(time, curVal));
    attach(plot);
}

PlotCurveItem::~PlotCurveItem()
{

}

double PlotCurveItem::convertToDouble(std::string s)
{
    QByteArray bytes(s.c_str());
    return bytes.toDouble(); // returns 0.0 if conversion fails
}

void PlotCurveItem::setDisplay()
{
    mutex.lock();
    setSamples(datum_points);
    plot()->replot();
    mutex.unlock();
}

void PlotCurveItem::clearDisplay()
{

}

void PlotCurveItem::notifyNewValue(const DatumInfo* datum)
{
    mutex.lock();
    double time   = watched_datum->getLastTimestamp();
    double curVal = convertToDouble(watched_datum->getValue());
    datum_points.append(QPointF(time, curVal));
    mutex.unlock();

    DatumItem::notifyNewValue(datum); // call parent to trigger updates
}

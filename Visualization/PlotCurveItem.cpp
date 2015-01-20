#include "PlotCurveItem.h"
#include <QByteArray>

PlotCurveItem::PlotCurveItem(QwtPlot* plot, const DatumInfo* d, QColor color)
    : QwtPlotCurve(QString(d->getName().c_str())), DatumItem(d, true)
{
    QPen pen;
    pen.setColor(color);
    pen.setWidth(1);
    pen.setStyle(Qt::SolidLine);

    setStyle(QwtPlotCurve::Steps);  // Do not interopolate between points
    setCurveAttribute(QwtPlotCurve::Inverted, true); // step at new data
    setPen(pen);

    // Create a second curve for just highlighting the data points
    curve_dots = new QwtPlotCurve();
    pen.setWidth(5);
    curve_dots->setPen(pen);
    curve_dots->setStyle(QwtPlotCurve::Dots);
    curve_dots->setItemAttribute(QwtPlotItem::Legend, false);

    // Create a third curve for just highlighting the data points that have
    // gone above or below the max/min
    bad_value_dots = new QwtPlotCurve();
    pen.setWidth(2);
    bad_value_dots->setPen(pen);
    bad_value_dots->setStyle(QwtPlotCurve::Sticks);
    bad_value_dots->setItemAttribute(QwtPlotItem::Legend, false);

    double time   = d->getLastTimestamp();
    double curVal = convertToDouble(d->getValue());
    datum_points.append(QPointF(time, curVal));
    if(d->isLessThanMin() || d->isGreaterThanMax())
        bad_points.append(QPointF(time, curVal));
    attach(plot);
    curve_dots->attach(plot);
    bad_value_dots->attach(plot);
}

PlotCurveItem::~PlotCurveItem()
{
    delete curve_dots;
    delete bad_value_dots;
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
    curve_dots->setSamples(datum_points);
    bad_value_dots->setSamples(bad_points);
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
    if(datum->isLessThanMin() || datum->isGreaterThanMax())
        bad_points.append(QPointF(time, curVal));
    mutex.unlock();

    DatumItem::notifyNewValue(datum); // call parent to trigger updates
}

// Plot all available datapoints instead of
// just what came in since creation (default)
void PlotCurveItem::showFullHistory()
{
    mutex.lock();
    datum_points.clear();
    bad_points.clear();
    std::map<double, std::string> hist = watched_datum->getHistory();
    std::map<double, std::string>::const_iterator it;
    for(it = hist.begin(); it != hist.end(); it++)
    {
        double time = it->first;
        double val  = convertToDouble(it->second);
        datum_points.append(QPointF(time, val));
        if(watched_datum->isLessThanMin(time) ||
           watched_datum->isGreaterThanMax(time))
        {
            bad_points.append(QPointF(time, val));
        }
    }
    mutex.unlock();

    setDisplay();
}

void PlotCurveItem::truncateHistory()
{
    mutex.lock();
    datum_points.clear();
    bad_points.clear();
    mutex.unlock();

    setDisplay();
}

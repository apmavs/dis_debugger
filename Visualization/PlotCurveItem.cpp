#include "PlotCurveItem.h"
#include <QByteArray>

PlotCurveItem::PlotCurveItem(QwtPlot* plot, const DatumInfo* d, QColor c)
    : QwtPlotCurve(QString(d->getName().c_str())), DatumItem(d, true)
{
    QPen pen;
    color = c;
    pen.setColor(c);
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

bool PlotCurveItem::equivalentTo(const PlotCurveItem* rhs) const
{
    bool ret = DatumItem::equivalentTo(rhs);
    if(color != rhs->color)
        ret = false;

    return ret;
}

QString PlotCurveItem::getStringRepresentation() const
{
    QString rep("<PlotCurveItem>\n");
    if(watched_datum != NULL)
    {
        rep += "<WatchedDatum>" +
               QString(watched_datum->getStringRepresentation().c_str()) +
               "</WatchedDatum>\n";
    }
    else
        std::cerr << __FILE__ << ":ERROR watched_datum NULL" << std::endl;

    int red, green, blue, alpha;
    color.getRgb(&red, &green, &blue, &alpha);
    rep += "<ColorRed>"   + QString::number(red)   + "</ColorRed>\n";
    rep += "<ColorGreen>" + QString::number(green) + "</ColorGreen>\n";
    rep += "<ColorBlue>"  + QString::number(blue)  + "</ColorBlue>\n";
    rep += "<ColorAlpha>" + QString::number(alpha) + "</ColorAlpha>\n";

    rep += "</PlotCurveItem>\n";

    return rep;
}

PlotCurveItem* PlotCurveItem::createFromStringRepresentation(QString rep,
                                                     QwtPlot* p)
{
    std::string guts = Configuration::getTagValue(rep.toStdString(),
                                                  "PlotCurveItem");
    std::string datumRep  = Configuration::getTagValue(guts, "WatchedDatum");
    DatumInfo* watchDatum = DataModelController::getInstance()->
                                        getDatumInfoPtr(datumRep);
    QString red   = QString(Configuration::getTagValue(guts, "ColorRed").c_str());
    QString green = QString(Configuration::getTagValue(guts, "ColorGreen").c_str());
    QString blue  = QString(Configuration::getTagValue(guts, "ColorBlue").c_str());
    QString alpha = QString(Configuration::getTagValue(guts, "ColorAlpha").c_str());
    int r = red.toInt();
    int g = green.toInt();
    int b = blue.toInt();
    int a = alpha.toInt();
    QColor c(r, g, b, a);
    PlotCurveItem* ret = new PlotCurveItem(p, watchDatum, c);

    return ret;
}

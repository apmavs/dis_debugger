#include "gtest/gtest.h"
#include <QApplication>

#include "DatumInfo.h"
#include "../Visualization/PlotCurveItem.h"


TEST(PlotCurveItemTest, RepresentationTest)
{
    // Create QApplication so that we can create QwtPlot which is a widget
    int argc;
    QApplication app(argc, NULL);
    app.closeAllWindows();

    QwtPlot TestPlot;
    DatumInfo* datum1 = DatumInfo::createDatum(55.0, "uint8", QByteArray(1, 'c'));
    DatumInfo* datum2 = DatumInfo::createDatum(45.0, "int8", QByteArray(1, 'c'));
    PlotCurveItem* item1 = new PlotCurveItem(&TestPlot, datum1, QColor(255, 0, 0));
    PlotCurveItem* item2 = new PlotCurveItem(&TestPlot, datum2, QColor(255, 0, 0));

    QString rep1 = item1->getStringRepresentation();
    QString rep2 = item2->getStringRepresentation();

    PlotCurveItem* repItem1 = PlotCurveItem::createFromStringRepresentation(rep1, &TestPlot);
    PlotCurveItem* repItem2 = PlotCurveItem::createFromStringRepresentation(rep2, &TestPlot);

    EXPECT_TRUE (item1->equivalentTo(repItem1));
    EXPECT_FALSE(item1->equivalentTo(repItem2));

    EXPECT_FALSE(item2->equivalentTo(repItem1));
    EXPECT_TRUE (item2->equivalentTo(repItem2));

    delete item1;
    delete item2;
    delete repItem1;
    delete repItem2;
    delete datum1;
    delete datum2;
}


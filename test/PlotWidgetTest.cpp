#include "gtest/gtest.h"
#include <QApplication>

#include "DatumInfo.h"
#include "../Visualization/EntityDatumItem.h"
#include "../Visualization/PlotWidget.h"


TEST(PlotWidgetTest, RepresentationTest)
{
    // Create QApplication so that we can create widgets
    int argc;
    QApplication app(argc, NULL);
    app.closeAllWindows();

    DatumInfo* datum1 = DatumInfo::createDatum(55.0, "uint8", QByteArray(1, 'c'));
    DatumInfo* datum2 = DatumInfo::createDatum(45.0, "int8", QByteArray(1, '&'));
    EntityDatumItem* item1 = new EntityDatumItem(NULL, datum1);
    EntityDatumItem* item2 = new EntityDatumItem(NULL, datum2);

    PlotWidget TestPlotWidget(NULL);
    TestPlotWidget.addCurve(item1);
    TestPlotWidget.addCurve(item2);

    QString rep = TestPlotWidget.getStringRepresentation();

    PlotWidget* repWidget = PlotWidget::createFromStringRepresentation(rep);

    EXPECT_TRUE (TestPlotWidget.equivalentTo(repWidget));

    delete item1;
    delete item2;
    delete repWidget;
    delete datum1;
    delete datum2;
}



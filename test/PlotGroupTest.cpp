#include "gtest/gtest.h"
#include <QApplication>

#include "DatumInfo.h"
#include "../Visualization/EntityDatumItem.h"
#include "../Visualization/PlotGroupBox.h"


TEST(PlotGroupTest, RepresentationTest)
{
    // Create QApplication so that we can create widgets
    int argc = 0;
    QApplication app(argc, NULL);
    app.closeAllWindows();

    DatumInfo* datum1 = DatumInfo::createDatum(55.0, "uint8", QByteArray(1, 'c'));
    DatumInfo* datum2 = DatumInfo::createDatum(45.0, "int8", QByteArray(1, '&'));
    EntityDatumItem* item1 = new EntityDatumItem(NULL, datum1);
    EntityDatumItem* item2 = new EntityDatumItem(NULL, datum2);

    PlotWidget* TestPlotWidget1  = new PlotWidget(NULL);
    PlotWidget* TestPlotWidget2  = new PlotWidget(NULL);
    PlotWidget* TestPlotWidget3  = new PlotWidget(NULL);
    PlotWidget* TestPlotWidget4  = new PlotWidget(NULL);
    PlotWidget* TestPlotWidget5  = new PlotWidget(NULL);
    PlotWidget* TestPlotWidget6  = new PlotWidget(NULL);
    PlotWidget* TestPlotWidget7  = new PlotWidget(NULL);
    PlotWidget* TestPlotWidget8  = new PlotWidget(NULL);
    PlotWidget* TestPlotWidget9  = new PlotWidget(NULL);
    PlotWidget* TestPlotWidget10 = new PlotWidget(NULL);
    TestPlotWidget1->addCurve(item1);
    TestPlotWidget1->addCurve(item2);
    TestPlotWidget2->addCurve(item2);
    TestPlotWidget3->addCurve(item1);

    TestPlotWidget5->addCurve(item2);

    TestPlotWidget6->addCurve(item1);
    TestPlotWidget6->addCurve(item2);
    TestPlotWidget7->addCurve(item2);
    TestPlotWidget8->addCurve(item1);

    TestPlotWidget10->addCurve(item1);
    TestPlotWidget10->addCurve(item2);

    PlotGroupBox* TestBox1 = new PlotGroupBox();
    PlotGroupBox* TestBox2 = new PlotGroupBox();
    PlotGroupBox* TestBox3 = new PlotGroupBox();
    PlotGroupBox* TestBox4 = new PlotGroupBox();

    QList<QTreeWidgetItem*> firstItems;
    firstItems.append(item1);
    firstItems.append(item2);

    TestBox1->addToFirstPlot(firstItems);
    TestBox1->addPlot(TestPlotWidget1);
    TestBox1->addPlot(TestPlotWidget2);
    TestBox1->addPlot(TestPlotWidget3);
    TestBox1->addPlot(TestPlotWidget4);

    TestBox2->addPlot(TestPlotWidget5);

    TestBox3->addPlot(TestPlotWidget6);
    TestBox3->addPlot(TestPlotWidget7);
    TestBox3->addPlot(TestPlotWidget8);
    TestBox3->addPlot(TestPlotWidget9);

    // Keep TestBox4 empty

    QString rep1 = TestBox1->getStringRepresentation();
    QString rep2 = TestBox2->getStringRepresentation();
    QString rep3 = TestBox3->getStringRepresentation();
    QString rep4 = TestBox4->getStringRepresentation();

    PlotGroupBox* repBox1 = new PlotGroupBox();
    PlotGroupBox* repBox2 = new PlotGroupBox();
    PlotGroupBox* repBox3 = new PlotGroupBox();
    PlotGroupBox* repBox4 = new PlotGroupBox();

    // Test to make sure setFromStringRep clears old plot widgets
    repBox4->addPlot(TestPlotWidget10);

    repBox1->setFromStringRepresentation(rep1);
    repBox2->setFromStringRepresentation(rep2);
    repBox3->setFromStringRepresentation(rep3);
    repBox4->setFromStringRepresentation(rep4);

    EXPECT_TRUE (TestBox1->equivalentTo(repBox1));
    EXPECT_FALSE(TestBox1->equivalentTo(repBox2));
    EXPECT_FALSE(TestBox1->equivalentTo(repBox3));
    EXPECT_FALSE(TestBox1->equivalentTo(repBox4));

    EXPECT_FALSE(TestBox2->equivalentTo(repBox1));
    EXPECT_TRUE (TestBox2->equivalentTo(repBox2));
    EXPECT_FALSE(TestBox2->equivalentTo(repBox3));
    EXPECT_FALSE(TestBox2->equivalentTo(repBox4));

    EXPECT_FALSE(TestBox3->equivalentTo(repBox1));
    EXPECT_FALSE(TestBox3->equivalentTo(repBox2));
    EXPECT_TRUE (TestBox3->equivalentTo(repBox3));
    EXPECT_FALSE(TestBox3->equivalentTo(repBox4));

    EXPECT_FALSE(TestBox4->equivalentTo(repBox1));
    EXPECT_FALSE(TestBox4->equivalentTo(repBox2));
    EXPECT_FALSE(TestBox4->equivalentTo(repBox3));
    EXPECT_TRUE (TestBox4->equivalentTo(repBox4));

    delete TestBox1;
    delete TestBox2;
    delete TestBox3;
    delete TestBox4;
    delete repBox1;
    delete repBox2;
    delete repBox3;
    delete repBox4;
    delete item1;
    delete item2;
    delete datum1;
    delete datum2;
}



#include "gtest/gtest.h"
#include <QApplication>

#include "DatumInfo.h"
#include "../Visualization/EntityDatumItem.h"
#include "../Visualization/WatchList.h"


TEST(WatchListTest, RepresentationTest)
{
    // Create QApplication so that we can create widgets
    int argc = 0;
    QApplication app(argc, NULL);
    app.closeAllWindows();

    DatumInfo* datum1 = DatumInfo::createDatum(55.0, "uint8", QByteArray(1, 'c'));
    DatumInfo* datum2 = DatumInfo::createDatum(45.0, "int8", QByteArray(1, '&'));
    EntityDatumItem* item1 = new EntityDatumItem(NULL, datum1);
    EntityDatumItem* item2 = new EntityDatumItem(NULL, datum2);
    QList<QTreeWidgetItem*> items;
    items.append(item1);
    items.append(item2);

    WatchList* TestList = new WatchList(NULL);
    TestList->addItems(items, QModelIndex());

    QString rep = TestList->getStringRepresentation();

    WatchList* repList = WatchList::createFromStringRepresentation(rep, NULL);

    EXPECT_TRUE (TestList->equivalentTo(repList));

    delete TestList;
    delete repList;
    delete datum1;
    delete datum2;
}



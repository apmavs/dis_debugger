#include "gtest/gtest.h"

#include "DatumInfo.h"
#include "../Visualization/WatchDatumItem.h"


TEST(WatchDatumItemTest, RepresentationTest)
{
    DatumInfo* datum = DatumInfo::createDatum(55.0, "uint8", QByteArray(1, 'c'));
    EntityDatumItem* eitem1 = new EntityDatumItem(NULL, datum);
    EntityDatumItem* eitem2 = new EntityDatumItem(NULL, "CatName");
    EntityDatumItem* eitem3 = new EntityDatumItem(eitem1, datum);
    EntityDatumItem* eitem4 = new EntityDatumItem(eitem2, "CatName2");
    WatchDatumItem* item1 = WatchDatumItem::createWatchItem(eitem1);
    WatchDatumItem* item2 = WatchDatumItem::createWatchItem(eitem2);
    WatchDatumItem* item3 = WatchDatumItem::createWatchItem(eitem3);
    WatchDatumItem* item4 = WatchDatumItem::createWatchItem(eitem4);

    QString rep1 = item1->getStringRepresentation();
    QString rep2 = item2->getStringRepresentation();
    QString rep3 = item3->getStringRepresentation();
    QString rep4 = item4->getStringRepresentation();

    WatchDatumItem* repItem1 = WatchDatumItem::createFromStringRepresentation(rep1);
    WatchDatumItem* repItem2 = WatchDatumItem::createFromStringRepresentation(rep2);
    WatchDatumItem* repItem3 = WatchDatumItem::createFromStringRepresentation(rep3);
    WatchDatumItem* repItem4 = WatchDatumItem::createFromStringRepresentation(rep4);

    EXPECT_TRUE (item1->equivalentTo(repItem1));
    EXPECT_FALSE(item1->equivalentTo(repItem2));
    EXPECT_FALSE(item1->equivalentTo(repItem3));
    EXPECT_FALSE(item1->equivalentTo(repItem4));

    EXPECT_FALSE(item2->equivalentTo(repItem1));
    EXPECT_TRUE (item2->equivalentTo(repItem2));
    EXPECT_FALSE(item2->equivalentTo(repItem3));
    EXPECT_FALSE(item2->equivalentTo(repItem4));

    EXPECT_FALSE(item3->equivalentTo(repItem1));
    EXPECT_FALSE(item3->equivalentTo(repItem2));
    EXPECT_TRUE (item3->equivalentTo(repItem3));
    EXPECT_FALSE(item3->equivalentTo(repItem4));

    EXPECT_FALSE(item4->equivalentTo(repItem1));
    EXPECT_FALSE(item4->equivalentTo(repItem2));
    EXPECT_FALSE(item4->equivalentTo(repItem3));
    EXPECT_TRUE (item4->equivalentTo(repItem4));

    delete eitem1;
    delete eitem2;
    delete item1;
    delete item2;
    delete repItem1;
    delete repItem2;
    delete datum;
}


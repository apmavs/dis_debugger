#include "gtest/gtest.h"

#include "DatumInfo.h"
#include "../Visualization/EntityDatumItem.h"


TEST(EntityDatumItemTest, RepresentationTest)
{
    DatumInfo* datum = DatumInfo::createDatum(55.0, "uint8", QByteArray(1, 'c'));
    EntityDatumItem* item1 = new EntityDatumItem(NULL, datum);
    EntityDatumItem* item2 = new EntityDatumItem(NULL, "CatName");
    EntityDatumItem* item3 = new EntityDatumItem(item1, datum);
    EntityDatumItem* item4 = new EntityDatumItem(item2, "CatName2");

    QString rep1 = item1->getStringRepresentation();
    QString rep2 = item2->getStringRepresentation();
    QString rep3 = item3->getStringRepresentation();
    QString rep4 = item4->getStringRepresentation();

    EntityDatumItem* repItem1 = EntityDatumItem::createFromStringRepresentation(rep1);
    EntityDatumItem* repItem2 = EntityDatumItem::createFromStringRepresentation(rep2);
    EntityDatumItem* repItem3 = EntityDatumItem::createFromStringRepresentation(rep3);
    EntityDatumItem* repItem4 = EntityDatumItem::createFromStringRepresentation(rep4);

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

    delete item1;
    delete item2;
    delete repItem1;
    delete repItem2;
    delete datum;
}

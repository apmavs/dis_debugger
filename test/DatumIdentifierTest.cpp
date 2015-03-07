#include "DatumIdentifier.h"

#include "gtest/gtest.h"

// Test to check that comparisons for similar and different
// identifiers work correctly
TEST(DatumIdentifierTest, CompareTest)
{
    DatumIdentifier id1, id2;
    id1.setApp(1);
    id1.setEntity(3);
    id1.setSite(4);
    id2.setApp(1);
    id2.setEntity(3);
    id2.setSite(4);

    EXPECT_TRUE(id1 == id2);

    id1.setApp(0);
    EXPECT_FALSE(id1 == id2);
    id1.setApp(1);

    id1.setEntity(0);
    EXPECT_FALSE(id1 == id2);
    id1.setEntity(3);

    id1.setSite(0);
    EXPECT_FALSE(id1 == id2);
    id1.setSite(4);

    EXPECT_TRUE(id1 == id2);
}

// Test to check that to and from string representations
// work correctly
TEST(DatumIdentifierTest, RepresentationTest)
{
    DatumIdentifier id1, id2;
    id1.setApp(1);
    id1.setEntity(2);
    id1.setSite(3);
    id2.setApp(4);
    id2.setEntity(5);
    id2.setSite(6);

    EXPECT_FALSE(id1 == id2);

    std::string rep1 = id1.getStringRepresentation();
    id2 = DatumIdentifier::fromStringRepresentation(rep1);
    EXPECT_TRUE(id1 == id2);

    std::string rep2 = id2.getStringRepresentation();
    EXPECT_TRUE(rep1 == rep2);
}

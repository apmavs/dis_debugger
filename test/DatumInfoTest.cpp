#include "DatumInfo.h"

#include "gtest/gtest.h"

static const std::string UNIT1 = "UNIT1", UNIT2 = "UNIT2";
static const std::string UNIT_CLASS1 = "UNIT_CLASS1", UNIT_CLASS2 = "UNIT_CLASS2";
static const std::string NAME1 = "NAME1", NAME2 = "NAME2";
static const std::string CAT1 = "CAT1", CAT2 = "CAT2";
static const std::string DESC1 = "DESC1", DESC2 = "DESC2";
static const uint8_t MIN = 5, MAX = 10;
class DatumInfoTestClass
{
public:
    DatumInfo* datum1;
    DatumInfo* datum2;
    DatumIdentifier id1, id2;
    Uint8Value min1, min2, max1, max2;

    DatumInfoTestClass()
    {
        datum1 = DatumInfo::createDatum(1.0, "uint8", QByteArray());
        datum2 = DatumInfo::createDatum(1.0, "uint8", QByteArray());
        id1.setApp(1);
        id1.setEntity(3);
        id1.setSite(4);
        id2.setApp(1);
        id2.setEntity(3);
        id2.setSite(4);
        min1.setValue(QByteArray(1, (char)MIN));
        max1.setValue(QByteArray(1, (char)MAX));
        min2.setValue(QByteArray(1, (char)MIN));
        max2.setValue(QByteArray(1, (char)MAX));
        datum1->setMinimum(&min1);
        datum1->setMaximum(&max1);
        datum2->setMinimum(&min2);
        datum2->setMaximum(&max2);
        datum1->setUnit(UNIT1);
        datum1->setUnitClass(UNIT_CLASS1);
        datum1->setName(NAME1);
        datum1->setCategory(CAT1);
        datum1->setDescription(DESC1);
        datum2->setUnit(UNIT2);
        datum2->setUnitClass(UNIT_CLASS2);
        datum2->setName(NAME2);
        datum2->setCategory(CAT2);
        datum2->setDescription(DESC2);
        datum1->setId(id1);
        datum2->setId(id2);
    }

    ~DatumInfoTestClass()
    {
        delete datum1;
        delete datum2;
    }
};


// Basic test to make sure get/sets work
TEST(DatumInfoTest, BasicTest)
{
    DatumInfoTestClass test;
    EXPECT_EQ(test.datum1->getUnit(),        UNIT1);
    EXPECT_EQ(test.datum1->getUnitClass(),   UNIT_CLASS1);
    EXPECT_EQ(test.datum1->getName(),        NAME1);
    EXPECT_EQ(test.datum1->getCategory(),    CAT1);
    EXPECT_EQ(test.datum1->getDescription(), DESC1);
    EXPECT_EQ(test.datum1->getType(),        "uint8");
    EXPECT_TRUE(test.datum1->getId()   ==    test.id1);
    EXPECT_FALSE(test.datum1->equivalentTo(test.datum2));

    // Set datum2 parameters to datum1's data to make sure equivalent
    // routine works correctly
    test.datum2->setUnit(UNIT1);
    test.datum2->setUnitClass(UNIT_CLASS1);
    test.datum2->setName(NAME1);
    test.datum2->setCategory(CAT1);
    test.datum2->setDescription(DESC1);
    // MUST be created with the same type
    test.datum2->setId(test.id1);
    test.datum2->setMinimum(&test.min1);
    test.datum2->setMaximum(&test.max1);
    EXPECT_TRUE(test.datum1->equivalentTo(test.datum2));
}

// Test that min and max violations are detected
TEST(DatumInfoTest, MinMaxTest)
{
    DatumInfoTestClass test;
    QByteArray value(1, (char)6);
    test.datum1->addValue(1.0, value);
    EXPECT_FALSE(test.datum1->isLessThanMin());
    EXPECT_FALSE(test.datum1->isGreaterThanMax());
    value = QByteArray(1, (char)0);
    test.datum1->addValue(2.0, value);
    EXPECT_TRUE(test.datum1->isLessThanMin());
    EXPECT_FALSE(test.datum1->isGreaterThanMax());
    value = QByteArray(1, (char)50);
    test.datum1->addValue(3.0, value);
    EXPECT_FALSE(test.datum1->isLessThanMin());
    EXPECT_TRUE(test.datum1->isGreaterThanMax());
    value = QByteArray(1, (char)7);
    test.datum1->addValue(4.0, value);

    // Check specific timestamp checks
    EXPECT_FALSE(test.datum1->isLessThanMin(1.0));
    EXPECT_FALSE(test.datum1->isGreaterThanMax(1.0));
    EXPECT_TRUE(test.datum1->isLessThanMin(2.0));
    EXPECT_FALSE(test.datum1->isGreaterThanMax(2.0));
    EXPECT_FALSE(test.datum1->isLessThanMin(3.0));
    EXPECT_TRUE(test.datum1->isGreaterThanMax(3.0));
}

// Test history and ability to truncate it
TEST(DatumInfoTest, HistoryTest)
{
    DatumInfoTestClass test;
    EXPECT_EQ(0, test.datum1->getHistory().size());
    QByteArray value(1, (char)6);
    test.datum1->addValue(1.0, value);
    EXPECT_EQ(1, test.datum1->getHistory().size());
    value = QByteArray(1, (char)7);
    test.datum1->addValue(2.0, value);
    EXPECT_EQ(2, test.datum1->getHistory().size());
    value = QByteArray(1, (char)8);
    test.datum1->addValue(3.0, value);
    EXPECT_EQ(3, test.datum1->getHistory().size());
    value = QByteArray(1, (char)9);
    test.datum1->addValue(4.0, value);
    EXPECT_EQ(4, test.datum1->getHistory().size());
    value = QByteArray(1, (char)10);
    test.datum1->addValue(5.0, value);
    EXPECT_EQ(5, test.datum1->getHistory().size());

    // Test truncate
    test.datum1->truncateHistory(3.0); // Should truncate times 4 and 5
    EXPECT_EQ(3, test.datum1->getHistory().size());
    EXPECT_EQ("8", test.datum1->getValue()); // Last value before truncate time

    // Check a value in the history
    std::map<double, std::string> history = test.datum1->getHistory();
    ASSERT_TRUE(history.count(2.0) == 1);
    EXPECT_EQ("7", history[2.0]);
}

// Test string representation functionality
TEST(DatumInfoTest, RepresentationTest)
{
    DatumInfoTestClass test;
    DatumInfo* duplicate;
    duplicate = DatumInfo::createDatum(666.333, "double", QByteArray());
    EXPECT_FALSE(test.datum1->equivalentTo(duplicate));
    EXPECT_FALSE(test.datum2->equivalentTo(duplicate));
    delete duplicate;

    std::string rep1 = test.datum1->getStringRepresentation();
    std::string rep2 = test.datum2->getStringRepresentation();

    duplicate = DatumInfo::createFromStringRepresentation(rep1);
    EXPECT_TRUE(test.datum1->equivalentTo(duplicate));
    EXPECT_FALSE(test.datum2->equivalentTo(duplicate));

    duplicate = DatumInfo::createFromStringRepresentation(rep2);
    EXPECT_FALSE(test.datum1->equivalentTo(duplicate));
    EXPECT_TRUE(test.datum2->equivalentTo(duplicate));
}

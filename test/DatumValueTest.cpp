#include "gtest/gtest.h"
#include <sstream>

#include "DatumValue.h"

TEST(DatumValueTest, CreateCopyTest)
{
    uint64_t val64 = 523895732842;
    double time = 234.55;
    const char* valPtr = (const char*)(&val64);
    QByteArray valArray(valPtr, 8);
    DatumValue* value = DatumValue::create(valArray, "uint64");
    value->setTimestamp(time);

    DatumValue* value2 = value->createCopy();

    std::ostringstream s;
    s << val64;
    EXPECT_EQ(s.str(), value->getValue());
    EXPECT_EQ(s.str(), value2->getValue());
    EXPECT_EQ(time, value->getTimestamp());
    EXPECT_EQ(time, value2->getTimestamp());
}

TEST(DatumValueTest, GreaterLessTest)
{
    uint64_t val1 = 2345235;
    uint64_t val2 = val1 + 500;
    uint64_t val3 = val1;

    const char* ptr1 = (const char*)(&val1);
    const char* ptr2 = (const char*)(&val2);
    const char* ptr3 = (const char*)(&val3);

    QByteArray array1(ptr1, 8);
    QByteArray array2(ptr2, 8);
    QByteArray array3(ptr3, 8);

    DatumValue* dval1 = DatumValue::create(array1, "uint64");
    DatumValue* dval2 = DatumValue::create(array2, "uint64");
    DatumValue* dval3 = DatumValue::create(array3, "uint64");

    EXPECT_TRUE(dval1->lessThan(dval2));
    EXPECT_FALSE(dval1->greaterThan(dval2));
    EXPECT_TRUE(dval2->greaterThan(dval1));
    EXPECT_FALSE(dval2->lessThan(dval1));

    EXPECT_FALSE(dval1->lessThan(dval3));
    EXPECT_FALSE(dval1->greaterThan(dval3));
    EXPECT_FALSE(dval3->lessThan(dval1));
    EXPECT_FALSE(dval3->greaterThan(dval1));

    delete dval1;
    delete dval2;
    delete dval3;
}

// Test that converted to and from string representations works
TEST(DatumValueTest, RepresentationTest)
{
    uint64_t valU64 = 328734243;
    QByteArray arrayU64((const char*)(&valU64), 8);
    int8_t   val8   = -4;
    QByteArray array8((const char*)(&val8), 1);
    std::string valStr = "This is a string for testing purposes\n";

    DatumValue* u64val = DatumValue::create(arrayU64, "uint64");
    DatumValue* i8val  = DatumValue::create(array8, "int8");
    DatumValue* strval = DatumValue::create(valStr, "string");
    u64val->setTimestamp(-33.0);
    i8val->setTimestamp(52345234.0);
    strval->setTimestamp(0.0);

    std::string rep64  = u64val->getStringRepresentation();
    std::string rep8   = i8val ->getStringRepresentation();
    std::string repStr = strval->getStringRepresentation();

    DatumValue *u64val2 = DatumValue::createFromStringRepresentation(rep64);
    DatumValue *i8val2  = DatumValue::createFromStringRepresentation(rep8);
    DatumValue *strval2 = DatumValue::createFromStringRepresentation(repStr);

    EXPECT_FALSE(u64val->greaterThan(u64val2));
    EXPECT_FALSE(u64val->lessThan(u64val2));
    EXPECT_EQ(u64val->getValue(), u64val2->getValue());
    EXPECT_EQ(u64val->getTimestamp(), u64val2->getTimestamp());
    EXPECT_FALSE(i8val->greaterThan(i8val2));
    EXPECT_FALSE(i8val->lessThan(i8val2));
    EXPECT_EQ(i8val->getValue(), i8val2->getValue());
    EXPECT_EQ(i8val->getTimestamp(), i8val2->getTimestamp());
    EXPECT_EQ(valStr, strval->getValue());
    EXPECT_EQ(strval->getValue(), strval2->getValue());
    EXPECT_EQ(valStr, strval2->getValue());
    EXPECT_EQ(strval->getTimestamp(), strval->getTimestamp());


    // Create calls require deletes
    delete u64val;
    delete i8val;
    delete strval;
    delete u64val2;
    delete i8val2;
    delete strval2;
}

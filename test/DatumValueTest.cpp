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

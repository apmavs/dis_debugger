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

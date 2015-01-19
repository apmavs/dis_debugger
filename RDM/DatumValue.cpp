#include "DatumValue.h"

#include <QString>

DatumValue::DatumValue()
{
    timestamp = -1.0;
    mutex = new QMutex();
}

DatumValue::DatumValue(double t, QByteArray v)
{
    timestamp = t;
    value = v;
    mutex = new QMutex();
}

DatumValue::~DatumValue()
{
    delete mutex;
}

DatumValue* DatumValue::create(std::string v, std::string type)
{
    QByteArray val;
    const char* valPtr = NULL;
    uint8_t u8; uint16_t u16; uint32_t u32; uint64_t u64;
    int8_t  i8;  int16_t i16;  int32_t i32;  int64_t i64;
    float fl; double db;
    const char* str;
    int byteCount = 0;

    if(type == "uint8")
    {
        u8 = atoi(v.c_str());
        byteCount = 1;
        valPtr = (const char*)(&u8);
    }
    else if(type == "uint16")
    {
        u16 = atoi(v.c_str());
        byteCount = 2;
        valPtr = (const char*)(&u16);
    }
    else if(type == "uint32")
    {
        u32 = atoi(v.c_str());
        byteCount = 4;
        valPtr = (const char*)(&u32);
    }
    else if(type == "uint64")
    {
        u64 = atol(v.c_str());
        byteCount = 8;
        valPtr = (const char*)(&u64);
    }
    else if(type == "int8")
    {
        i8 = atoi(v.c_str());
        byteCount = 1;
        valPtr = (const char*)(&i8);
    }
    else if(type == "int16")
    {
        i16 = atoi(v.c_str());
        byteCount = 2;
        valPtr = (const char*)(&i16);
    }
    else if(type == "int32")
    {
        i32 = atoi(v.c_str());
        byteCount = 4;
        valPtr = (const char*)(&i32);
    }
    else if(type == "int64")
    {
        i64 = atol(v.c_str());
        byteCount = 8;
        valPtr = (const char*)(&i64);
    }
    else if(type == "float")
    {
        fl = atof(v.c_str());
        byteCount = 4;
        valPtr = (const char*)(&fl);
    }
    else if(type == "double")
    {
        db = atof(v.c_str());
        byteCount = 8;
        valPtr = (const char*)(&db);
    }
    else if(type == "string")
    {
        str = v.c_str();
        byteCount = v.size() + 1;
        valPtr = (const char*)(&str);
    }

    for(int i = 0; i < byteCount; i++)
    {
        val.append(*valPtr);
        valPtr++;
    }

    return create(val, type);
}

DatumValue* DatumValue::create(QByteArray v, std::string type)
{
    DatumValue* newVal;
    if(     type == "uint8" )   newVal = new Uint8Value();
    else if(type == "uint16")   newVal = new Uint16Value();
    else if(type == "uint32")   newVal = new Uint32Value();
    else if(type == "uint64")   newVal = new Uint64Value();
    else if(type == "int8"  )   newVal = new Int8Value();
    else if(type == "int16" )   newVal = new Int16Value();
    else if(type == "int32" )   newVal = new Int32Value();
    else if(type == "int64" )   newVal = new Int64Value();
    else if(type == "float" )   newVal = new FloatValue();
    else if(type == "double")   newVal = new DoubleValue();
    else if(type == "string")   newVal = new StringValue();
    else                        newVal = new Uint8Value();
    newVal->value = v;

    return newVal;
}

void DatumValue::setTimestamp(double t)
{
    mutex->lock();
    timestamp = t;
    mutex->unlock();
}

void DatumValue::setValue(QByteArray v)
{
    mutex->lock();
    value = v;
    mutex->unlock();
}

double DatumValue::getTimestamp() const
{
    mutex->lock();
    double t = timestamp;
    mutex->unlock();
    return t;
}

QByteArray DatumValue::getRawData() const
{
    mutex->lock();
    QByteArray v = value;
    mutex->unlock();
    return v;
}


// ************************* StringValue *************************
StringValue::StringValue() : DatumValue() {}
StringValue::~StringValue(){}

std::string StringValue::getValue() const
{
    QString val = "";
    bool foundNullTerminator = false;
    mutex->lock();
    for(int index = 0; index < value.size(); index++)
    {
        if(value.at(index) == '\0')
        {
            foundNullTerminator = true;
            break;
        }
    }
    if(foundNullTerminator)
        val = QString(value.constData());
    else
        val = "Error:NoNull!";
    mutex->unlock();
    return val.toStdString();
}

DatumValue* StringValue::createCopy() const
{
    StringValue* copy = new StringValue();
    mutex->lock();
    copy->value     = value;
    copy->timestamp = timestamp;
    mutex->unlock();
    return copy;
}

bool StringValue::lessThan(DatumValue* rhs) const
{
    std::string val1 = getValue();
    std::string val2 = rhs->getValue();
    return (val1 < val2);
}

bool StringValue::greaterThan(DatumValue* rhs) const
{
    std::string val1 = getValue();
    std::string val2 = rhs->getValue();
    return (val1 > val2);
}
// ************************* StringValue *************************

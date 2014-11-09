#include "DatumValue.h"
#include <QString>
#include <stdint.h>

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

DatumValue & DatumValue::operator=(const DatumValue& copyVal)
{
    timestamp = copyVal.getTimestamp();
    value     = copyVal.getRawData();
    mutex     = new QMutex();

    return *this;
}

DatumValue::DatumValue(const DatumValue& copyVal)
{
    timestamp = copyVal.getTimestamp();
    value     = copyVal.getRawData();
    mutex     = new QMutex();
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


// ************************* Uint8Value *************************
Uint8Value::Uint8Value(){}
Uint8Value::~Uint8Value(){}

std::string Uint8Value::getValue() const
{
    uint8_t val = 0;
    mutex->lock();
    if(value.size() >= 1)
        val = *((uint8_t*)value.constData());
    mutex->unlock();
    return QString("%1").arg(val).toStdString();
}
// ************************* Uint8Value *************************

// ************************* Uint16Value *************************
Uint16Value::Uint16Value(){}
Uint16Value::~Uint16Value(){}

std::string Uint16Value::getValue() const
{
    uint16_t val = 0;
    mutex->lock();
    if(value.size() >= 2)
        val = *((uint16_t*)value.constData());
    mutex->unlock();
    return QString("%1").arg(val).toStdString();
}
// ************************* Uint32Value *************************

// ************************* Uint32Value *************************
Uint32Value::Uint32Value(){}
Uint32Value::~Uint32Value(){}

std::string Uint32Value::getValue() const
{
    uint32_t val = 0;
    mutex->lock();
    if(value.size() >= 4)
        val = *((uint32_t*)value.constData());
    mutex->unlock();
    return QString("%1").arg(val).toStdString();
}
// ************************* Uint32Value *************************

// ************************* Uint64Value *************************
Uint64Value::Uint64Value(){}
Uint64Value::~Uint64Value(){}

std::string Uint64Value::getValue() const
{
    uint64_t val = 0;
    mutex->lock();
    if(value.size() >= 8)
        val = *((uint64_t*)value.constData());
    mutex->unlock();
    return QString("%1").arg(val).toStdString();
}
// ************************* Uint64Value *************************

// ************************* Int8Value *************************
Int8Value::Int8Value(){}
Int8Value::~Int8Value(){}

std::string Int8Value::getValue() const
{
    int8_t val = 0;
    mutex->lock();
    if(value.size() >= 1)
        val = *((int8_t*)value.constData());
    mutex->unlock();
    return QString("%1").arg(val).toStdString();
}
// ************************* Int8Value *************************

// ************************* Int16Value *************************
Int16Value::Int16Value(){}
Int16Value::~Int16Value(){}

std::string Int16Value::getValue() const
{
    int16_t val = 0;
    mutex->lock();
    if(value.size() >= 2)
        val = *((int16_t*)value.constData());
    mutex->unlock();
    return QString("%1").arg(val).toStdString();
}
// ************************* Int32Value *************************

// ************************* Int32Value *************************
Int32Value::Int32Value(){}
Int32Value::~Int32Value(){}

std::string Int32Value::getValue() const
{
    int32_t val = 0;
    mutex->lock();
    if(value.size() >= 4)
        val = *((int32_t*)value.constData());
    mutex->unlock();
    return QString("%1").arg(val).toStdString();
}
// ************************* Int32Value *************************

// ************************* Int64Value *************************
Int64Value::Int64Value(){}
Int64Value::~Int64Value(){}

std::string Int64Value::getValue() const
{
    int64_t val = 0;
    mutex->lock();
    if(value.size() >= 8)
        val = *((int64_t*)value.constData());
    mutex->unlock();
    return QString("%1").arg(val).toStdString();
}
// ************************* Int64Value *************************

// ************************* FloatValue *************************
FloatValue::FloatValue(){}
FloatValue::~FloatValue(){}

std::string FloatValue::getValue() const
{
    float val = 0.0f;
    mutex->lock();
    if(value.size() >= 4)
        val = *((float*)value.constData());
    mutex->unlock();
    return QString("%1").arg(val).toStdString();
}
// ************************* FloatValue *************************

// ************************* DoubleValue *************************
DoubleValue::DoubleValue(){}
DoubleValue::~DoubleValue(){}

std::string DoubleValue::getValue() const
{
    double val = 0.0;
    mutex->lock();
    if(value.size() >= 8)
        val = *((double*)(value.constData()));
    mutex->unlock();
    return QString("%1").arg(val).toStdString();
}
// ************************* DoubleValue *************************

// ************************* StringValue *************************
StringValue::StringValue(){}
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
    mutex->unlock();
    return val.toStdString();
}
// ************************* StringValue *************************

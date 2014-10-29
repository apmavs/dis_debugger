#include "DatumValue.h"

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
    value     = copyVal.getValue();
    mutex     = new QMutex();

    return *this;
}

DatumValue::DatumValue(const DatumValue& copyVal)
{
    timestamp = copyVal.getTimestamp();
    value     = copyVal.getValue();
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

QByteArray DatumValue::getValue() const
{
    mutex->lock();
    QByteArray v = value;
    mutex->unlock();
    return v;
}

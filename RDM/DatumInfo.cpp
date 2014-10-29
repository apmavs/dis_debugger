#include "DatumInfo.h"

DatumInfo::DatumInfo()
{
    type        = "";
    unit        = "";
    name        = "";
    category    = "";
    description = "";
}

void DatumInfo::setId(DatumIdentifier id)
{
    mutex.lock();
    identifier = id;
    mutex.unlock();
}

void DatumInfo::setType(std::string t)
{
    mutex.lock();
    type = t;
    mutex.unlock();
}

void DatumInfo::setUnit(std::string u)
{
    mutex.lock();
    unit = u;
    mutex.unlock();
}

void DatumInfo::setName(std::string n)
{
    mutex.lock();
    name = n;
    mutex.unlock();
}

void DatumInfo::setCategory(std::string c)
{
    mutex.lock();
    category = c;
    mutex.unlock();
}

void DatumInfo::setDescription(std::string d)
{
    mutex.lock();
    description = d;
    mutex.unlock();
}

DatumIdentifier DatumInfo::getId()
{
    mutex.lock();
    DatumIdentifier id = identifier;
    mutex.unlock();
    return id;
}

std::string DatumInfo::getType()
{
    mutex.lock();
    std::string t = type;
    mutex.unlock();
    return t;
}

std::string DatumInfo::getUnit()
{
    mutex.lock();
    std::string u = unit;
    mutex.unlock();
    return u;
}

std::string DatumInfo::getName()
{
    mutex.lock();
    std::string n = name;
    mutex.unlock();
    return n;
}

std::string DatumInfo::getCategory()
{
    mutex.lock();
    std::string c = category;
    mutex.unlock();
    return c;
}

std::string DatumInfo::getDescription()
{
    mutex.lock();
    std::string d = description;
    mutex.unlock();
    return d;
}

DatumValue DatumInfo::getValue()
{
    double time = -1.0;
    QByteArray data;

    mutex.lock();
    // Get most current value if any values exist
    std::map<double, DatumValue>::reverse_iterator it = values.rbegin();
    if(it != values.rend())
    {
        time = it->second.getTimestamp();
        data = it->second.getValue();
    }
    mutex.unlock();

    return DatumValue(time, data);
}

std::map<double, DatumValue> DatumInfo::getHistory()
{
    std::map<double, DatumValue> retMap;

    mutex.lock();
    retMap = values;
    mutex.unlock();

    return retMap;
}

void DatumInfo::addValue(DatumValue value)
{
    DatumValue newVal(value.getTimestamp(), value.getValue());
    mutex.lock();
    values[newVal.getTimestamp()] = value;
    mutex.unlock();
}

void DatumInfo::truncateHistory(double currentTime)
{
    std::map<double, DatumValue>::iterator it, itEnd;

    mutex.lock();
    itEnd = values.end();
    for(it = values.begin(); it != itEnd; it++)
    {
        if(it->first > currentTime)
        {
            values.erase(it, itEnd); // last element not erased
            values.erase(values.end());
            break;
        }
    }
    mutex.unlock();
}

#include "DatumInfo.h"
#include "PduDeconstructor.h"

#include <QString>

DatumInfo::DatumInfo()
{
    type        = "";
    unit        = "";
    name        = "";
    category    = "";
    description = "";
    mutex = new QMutex();
    has_minimum = false;
    has_maximum = false;
    minimum     = NULL;
    maximum     = NULL;
}

DatumInfo::~DatumInfo()
{
    int num = 0;
    while(!values.empty())
    {
        num++;
        DatumValue* v = values.back();
        values.pop_back();
        delete v;
    }
    delete mutex;
    if(minimum != NULL) delete minimum;
    if(maximum != NULL) delete maximum;
}

DatumValue* DatumInfo::getLastRawDatumValue()
{
    DatumValue* val = NULL;

    mutex->lock();
    // Get most current value if any values exist
    std::vector<DatumValue*>::reverse_iterator it = values.rbegin();
    if(it != values.rend())
        val = (*it);
    mutex->unlock();

    return val;
}

DatumInfo* DatumInfo::createDatum(double time, std::string type, QByteArray value)
{
    DatumInfo* datum = new DatumInfo();
    datum->type = type;
    datum->addValue(time, value);
    return datum;
}

double DatumInfo::getLastTimestamp() const
{
    double time = -1.0;

    mutex->lock();
    // Get most current value if any values exist
    std::vector<DatumValue*>::const_reverse_iterator it = values.rbegin();
    if(it != values.rend())
        time = (*it)->getTimestamp();
    mutex->unlock();

    return time;
}

QByteArray DatumInfo::getLastRawValue()
{
    QByteArray val = NULL;

    mutex->lock();
    // Get most current value if any values exist
    std::vector<DatumValue*>::reverse_iterator it = values.rbegin();
    if(it != values.rend())
        val = (*it)->getRawData();
    mutex->unlock();

    return val;
}

bool DatumInfo::hasSameId(DatumInfo* rhs)
{
    return (identifier == rhs->identifier);
}

void DatumInfo::setId(DatumIdentifier id)
{
    mutex->lock();
    identifier = id;
    mutex->unlock();
}

void DatumInfo::setUnit(std::string u)
{
    mutex->lock();
    unit = u;
    mutex->unlock();
}

void DatumInfo::setUnitClass(std::string u)
{
    mutex->lock();
    unit_class = u;
    mutex->unlock();
}

void DatumInfo::setName(std::string n)
{
    mutex->lock();
    name = n;
    mutex->unlock();
}

void DatumInfo::setCategory(std::string c)
{
    mutex->lock();
    category = c;
    mutex->unlock();
}

void DatumInfo::setDescription(std::string d)
{
    mutex->lock();
    description = d;
    mutex->unlock();
}

void DatumInfo::setMinimum(DatumValue* min)
{
    mutex->lock();
    if(minimum != NULL) delete minimum;
    minimum = min;
    has_minimum = true;
    mutex->unlock();
}

void DatumInfo::setMaximum(DatumValue* max)
{
    mutex->lock();
    if(maximum != NULL) delete maximum;
    maximum = max;
    has_maximum = true;
    mutex->unlock();
}

DatumIdentifier DatumInfo::getId() const
{
    mutex->lock();
    DatumIdentifier id = identifier;
    mutex->unlock();
    return id;
}

std::string DatumInfo::getEntityName() const
{
    DatumIdentifier id = getId();
    std::string entityName = PduDeconstructor::getEntityIdentifier(id.getSite(),
                                                                   id.getApp(),
                                                                   id.getEntity());
    return entityName;
}

std::string DatumInfo::getType() const
{
    mutex->lock();
    std::string t = type;
    mutex->unlock();
    return t;
}

std::string DatumInfo::getUnit() const
{
    mutex->lock();
    std::string u = unit;
    mutex->unlock();
    return u;
}

std::string DatumInfo::getUnitClass() const
{
    mutex->lock();
    std::string u = unit_class;
    mutex->unlock();
    return u;
}

std::string DatumInfo::getName() const
{
    mutex->lock();
    std::string n = name;
    mutex->unlock();
    return n;
}

std::string DatumInfo::getCategory() const
{
    mutex->lock();
    std::string c = category;
    mutex->unlock();
    return c;
}

std::string DatumInfo::getDescription() const
{
    mutex->lock();
    std::string d = description;
    mutex->unlock();
    return d;
}

// Get the most recent value
std::string DatumInfo::getValue() const
{
    std::string data;

    mutex->lock();
    // Get most current value if any values exist
    std::vector<DatumValue*>::const_reverse_iterator it = values.rbegin();
    if(it != values.rend())
        data = (*it)->getValue();

    mutex->unlock();

    return data;
}

std::map<double, std::string> DatumInfo::getHistory() const
{
    std::map<double, std::string> retVec;

    mutex->lock();
    std::vector<DatumValue*>::const_iterator it;
    for(it = values.begin(); it != values.end(); it++)
    {
        DatumValue* val = (*it);
        retVec[val->getTimestamp()] = val->getValue();
    }
    mutex->unlock();

    return retVec;
}

bool DatumInfo::isLessThanMin()
{
    // If no min defined, always return false
    bool lessThanMin = false;
    if(has_minimum)
    {
        DatumValue* curVal = getLastRawDatumValue();
        DatumValue* minVal = minimum;
        lessThanMin = curVal->lessThan(minVal);
    }
    return lessThanMin;
}

bool DatumInfo::isGreaterThanMax()
{
    // If no max defined, always return false
    bool greaterThanMax = false;
    if(has_maximum)
    {
        DatumValue* curVal = getLastRawDatumValue();
        DatumValue* maxVal = maximum;
        greaterThanMax = curVal->greaterThan(maxVal);
    }
    return greaterThanMax;
}

bool DatumInfo::addValue(double time, QByteArray value)
{
    bool valueChanged = false;
    DatumValue* newVal;

    mutex->lock();
    newVal = DatumValue::create(value, type);
    mutex->unlock();

    newVal->setTimestamp(time);
    newVal->setValue(value);

    // Only add new value if it is actually different
    QByteArray lastVal = getLastRawValue();
    mutex->lock();
    if(newVal->getRawData() != lastVal)
    {
        valueChanged = true;
        values.push_back(newVal);
    }
    else
    {
        delete newVal;
    }
    mutex->unlock();

    return valueChanged;
}

void DatumInfo::truncateHistory(double currentTime)
{
    std::vector<DatumValue*>::iterator it, itEnd;

    mutex->lock();
    itEnd = values.end();
    for(it = values.begin(); it != itEnd; it++)
    {
        if((*it)->getTimestamp() > currentTime)
        {
            std::vector<DatumValue*>::iterator delIt;
            for(delIt = it; delIt != itEnd; delIt++)
                delete (*delIt);
            values.erase(it, itEnd);
            break;
        }
    }
    mutex->unlock();
}

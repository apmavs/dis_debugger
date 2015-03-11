#include "DatumInfo.h"
#include "PduDeconstructor.h"
#include "Configuration.h"

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

bool DatumInfo::equivalentTo(DatumInfo* rhs)
{
    bool equiv = true; // Assume equal

    // If any definition part is different, then not equivalent
    if(identifier != rhs->identifier)
        equiv = false;
    else if(type != rhs->type)
        equiv = false;
    else if(unit != rhs->unit)
        equiv = false;
    else if(unit_class != rhs->unit_class)
        equiv = false;
    else if(name != rhs->name)
        equiv = false;
    else if(category != rhs->category)
        equiv = false;
    else if(description != rhs->description)
        equiv = false;
    else if(has_minimum && rhs->has_minimum &&
            minimum->getValue() != rhs->minimum->getValue())
        equiv = false;
    else if(has_maximum && rhs->has_maximum &&
            maximum->getValue() != rhs->maximum->getValue())
        equiv = false;
    else if(has_minimum != rhs->has_minimum)
        equiv = false;
    else if(has_maximum != rhs->has_maximum)
        equiv = false;

    return equiv;
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
    minimum = min->createCopy();
    has_minimum = true;
    mutex->unlock();
}

void DatumInfo::setMaximum(DatumValue* max)
{
    mutex->lock();
    if(maximum != NULL) delete maximum;
    maximum = max->createCopy();
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

bool DatumInfo::isLessThanMin() const
{
    return isLessThanMin(getLastTimestamp());
}

bool DatumInfo::isGreaterThanMax() const
{
    return isGreaterThanMax(getLastTimestamp());
}

bool DatumInfo::isLessThanMin(double timestamp) const
{
    DatumValue* checkVal = NULL;

    mutex->lock();
    // Get most current value if any values exist
    std::vector<DatumValue*>::const_iterator it;
    for(it = values.begin(); it != values.end(); it++)
    {
        if((*it)->getTimestamp() == timestamp)
        {
            checkVal = (*it);
            break;
        }
    }

    // If no min defined, always return false
    bool lessThanMin = false;
    if(has_minimum && (checkVal != NULL))
    {
        DatumValue* minVal = minimum;
        lessThanMin = checkVal->lessThan(minVal);
    }
    mutex->unlock();

    return lessThanMin;
}

bool DatumInfo::isGreaterThanMax(double timestamp) const
{
    DatumValue* checkVal = NULL;

    mutex->lock();
    // Get most current value if any values exist
    std::vector<DatumValue*>::const_iterator it;
    for(it = values.begin(); it != values.end(); it++)
    {
        if((*it)->getTimestamp() == timestamp)
        {
            checkVal = (*it);
            break;
        }
    }

    // If no max defined, always return false
    bool greaterThanMax = false;
    if(has_maximum && (checkVal != NULL))
    {
        DatumValue* maxVal = maximum;
        greaterThanMax = checkVal->greaterThan(maxVal);
    }
    mutex->unlock();

    return greaterThanMax;
}

bool DatumInfo::addValue(double time, QByteArray value)
{
    bool valueChanged = false;
    DatumValue* newVal = DatumValue::create(value, type);
    newVal->setTimestamp(time);

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

// Return value from tag <Tag>value</Tag>
// Returns empty string if not found
std::string getTagValue(std::string fromStr, std::string tag)
{
    std::string ret("");
    std::string beginTag("<" + tag + ">");
    std::string endTag("</" + tag + ">");
    size_t begin = fromStr.find(beginTag);
    if(begin != std::string::npos)
    {
        begin     += beginTag.length();
        size_t end = fromStr.find(endTag);
        if((end != std::string::npos) && (end > begin))
        {
            size_t tagLen = end - begin;
            ret = fromStr.substr(begin, tagLen);
        }
    }
    return ret;
}

// Return string representation so that object can be saved and loaded.
std::string DatumInfo::getStringRepresentation()
{
    mutex->lock();

    std::string rep("<DatumInfo>\n");
    rep += "<ID>" + identifier.getStringRepresentation() + "</ID>\n";
    rep += "<Type>"        + type +                        "</Type>\n";
    rep += "<Unit>"        + unit +                        "</Unit>\n";
    rep += "<UnitClass>"   + unit_class +                  "</UnitClass>\n";
    rep += "<Name>"        + name +                        "</Name>\n";
    rep += "<Category>"    + category +                    "</Category>\n";
    rep += "<Description>" + description +                 "</Description>\n";
    if(has_minimum)
        rep += "<Minimum>" + minimum->getStringRepresentation() + "</Minimum>\n";
    if(has_maximum)
        rep += "<Maximum>" + maximum->getStringRepresentation() + "</Maximum>\n";

    // Save off each stored value
    for(size_t i = 0; i < values.size(); i++)
        rep += "<Value>" + values[i]->getStringRepresentation() + "</Value>\n";


    rep += "</DatumInfo>\n";

    mutex->unlock();

    return rep;
}

DatumInfo* DatumInfo::createFromStringRepresentation(std::string rep)
{
    DatumInfo* ret = new DatumInfo();
    std::string idStr = Configuration::getTagValue(rep, "ID");
    ret->identifier    = DatumIdentifier::fromStringRepresentation(idStr);

    ret->type          = Configuration::getTagValue(rep, "Type");
    ret->unit          = Configuration::getTagValue(rep, "Unit");
    ret->unit_class    = Configuration::getTagValue(rep, "UnitClass");
    ret->name          = Configuration::getTagValue(rep, "Name");
    ret->category      = Configuration::getTagValue(rep, "Category");
    ret->description   = Configuration::getTagValue(rep, "Description");

    std::string minStr = Configuration::getTagValue(rep, "Minimum");
    if(minStr != "")
    {
        ret->has_minimum = true;
        ret->minimum = DatumValue::createFromStringRepresentation(minStr);
    }
    std::string maxStr = Configuration::getTagValue(rep, "Maximum");
    if(maxStr != "")
    {
        ret->has_maximum = true;
        ret->maximum = DatumValue::createFromStringRepresentation(maxStr);
    }

    // Get any stored values
    std::string valBeginTag = "<Value>";
    std::string valEndTag   = "</Value>";
    size_t valBegin = rep.find(valBeginTag);
    while(valBegin != std::string::npos)
    {
        valBegin += valBeginTag.length();
        size_t valEnd = rep.find(valEndTag);
        if(valEnd > valBegin)
        {
            size_t len = valEnd - valBegin;
            std::string valStr = rep.substr(valBegin, len);
            DatumValue* newVal = DatumValue::createFromStringRepresentation(valStr);
            ret->values.push_back(newVal);
        }

        // Consume byte we just parsed so we don't see it again
        rep = rep.substr(valEnd + 1);
        // Check if another byte is defined
        valBegin = rep.find("<Value>");
    }

    return ret;
}






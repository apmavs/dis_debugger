#ifndef DATUMINFO_H
#define DATUMINFO_H

#include "DatumValue.h"
#include "DatumIdentifier.h"

#include <QMutex>
#include <map>
#include <string>

class DatumInfo
{
private:
    QMutex mutex;
    DatumIdentifier identifier;
    std::string type;
    std::string unit;
    std::string name;
    std::string category;
    std::string description;
    std::map<double, DatumValue> values;

public:
    DatumInfo();

    void setId(DatumIdentifier id);
    void setType(std::string t);
    void setUnit(std::string u);
    void setName(std::string n);
    void setCategory(std::string c);
    void setDescription(std::string d);
    DatumIdentifier getId();
    std::string getType();
    std::string getUnit();
    std::string getName();
    std::string getCategory();
    std::string getDescription();
    DatumValue getValue();
    std::map<double, DatumValue> getHistory();
    void addValue(DatumValue value);
    void truncateHistory(double currentTime);
};

#endif // DATUMINFO_H

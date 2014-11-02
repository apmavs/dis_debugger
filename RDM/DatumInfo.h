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
    QMutex *mutex;
    DatumIdentifier identifier;
    std::string type;
    std::string unit;
    std::string unit_class;
    std::string name;
    std::string category;
    std::string description;
    std::map<double, DatumValue> values;

public:
    DatumInfo();
    ~DatumInfo();
    DatumInfo & operator=(const DatumInfo& copyVal);
    DatumInfo(const DatumInfo& copyVal);

    void setId(DatumIdentifier id);
    void setType(std::string t);
    void setUnit(std::string u);
    void setUnitClass(std::string u);
    void setName(std::string n);
    void setCategory(std::string c);
    void setDescription(std::string d);
    DatumIdentifier getId() const;
    std::string getType() const;
    std::string getUnit() const;
    std::string getUnitClass() const;
    std::string getName() const;
    std::string getCategory() const;
    std::string getDescription() const;
    DatumValue getValue();
    std::map<double, DatumValue> getHistory() const;
    void addValue(DatumValue value);
    void truncateHistory(double currentTime);
};

#endif // DATUMINFO_H

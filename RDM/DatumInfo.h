#ifndef DATUMINFO_H
#define DATUMINFO_H

#include "DatumValue.h"
#include "DatumIdentifier.h"

#include <QMutex>
#include <vector>
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
    std::vector<DatumValue*> values;
    DatumValue* minimum;
    DatumValue* maximum;
    bool has_minimum;
    bool has_maximum;

    DatumInfo();
    DatumInfo & operator=(const DatumInfo& copyVal);
    DatumInfo(const DatumInfo& copyVal);

protected:
    DatumValue* getLastRawDatumValue();

public:
    ~DatumInfo();

    static DatumInfo* createDatum(double time, std::string type, QByteArray value);
    double getLastTimestamp() const;
    QByteArray getLastRawValue();

    bool hasSameId(DatumInfo* rhs);
    void setId(DatumIdentifier id);
    void setUnit(std::string u);
    void setUnitClass(std::string u);
    void setName(std::string n);
    void setCategory(std::string c);
    void setDescription(std::string d);
    void setMinimum(DatumValue* min);
    void setMaximum(DatumValue* max);
    DatumIdentifier getId() const;
    std::string getEntityName() const;
    std::string getType() const;
    std::string getUnit() const;
    std::string getUnitClass() const;
    std::string getName() const;
    std::string getCategory() const;
    std::string getDescription() const;
    std::string getValue() const;
    std::map<double, std::string> getHistory() const;
    bool isLessThanMin();    // Current value is less than min
    bool isGreaterThanMax(); // Current value is greater than max

    bool addValue(double time, QByteArray value);
    void truncateHistory(double currentTime);
};

#endif // DATUMINFO_H

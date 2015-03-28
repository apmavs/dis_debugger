#ifndef UNITCLASSDEF_H
#define UNITCLASSDEF_H

#include <map>
#include <string>

class UnitDef {
public:
    std::string format;
    double factor;

    UnitDef() : format(""), factor(0.0) {}
    UnitDef(std::string format_, double factor_) : format(format_), factor(factor_) {}
};

class UnitClassDef
{
protected:
    std::string name;
    std::string defaultUnit;
    std::map<std::string, UnitDef> units;

public:
    UnitClassDef() : name(""), defaultUnit("") {}
    UnitClassDef(std::string name_, std::string defaultUnit_) : name(name_), defaultUnit(defaultUnit_) {}

    void addUnit(std::string name, UnitDef unit) { units.insert(std::make_pair(name, unit)); }

    bool isNull() { return !name.empty() && !units.empty(); }
    std::string getName() const {return name;};
    std::string getConversion(std::string fromUnit, std::string toUnit, double val);
};

#endif

#ifndef UNITCLASSDEF_H
#define UNITCLASSDEF_H

#include <iostream>
#include <map>
#include <string>
#include <sstream>
#include <cstdio>

typedef std::map<std::string, std::string> EnumDefMap;

class UnitDef
{
public:
    std::string format;
    double factor;
    EnumDefMap outputMap;

    UnitDef(std::string format_, double factor_, EnumDefMap outputMap_) :
        format(format_),
        factor(factor_),
        outputMap(outputMap_) {}
};

typedef std::map<std::string, UnitDef> UnitDefMap;

// Container for a list of unit definitions
class UnitClassDef
{
protected:
    std::string name;
    std::string defaultUnit;
    UnitDefMap units;

public:
    UnitClassDef() : name("DEFAULT"), defaultUnit("") {}
    UnitClassDef(std::string name_, std::string defaultUnit_) : name(name_), defaultUnit(defaultUnit_) {}

    void addUnit(std::string name, UnitDef unit) { units.insert(std::make_pair(name, unit)); }

    bool isNull() { return !name.empty() && !units.empty(); }

    std::string getName() const {return name;};
    const UnitDefMap & getUnits() const {return units;}

    // Get a string representing val in toUnit units
    template <typename T>
    std::string getConversion(const std::string & fromUnit, const std::string & toUnit, T val) const
    {
        const UnitDefMap::const_iterator fromIter = units.find(fromUnit.empty() ? defaultUnit : fromUnit);
        const UnitDefMap::const_iterator toIter = units.find(toUnit.empty() ? defaultUnit : toUnit);

        if(fromIter == units.end() || toIter == units.end())
        {
            return "Invalid conversion: (" + name + ") " + fromUnit + "->" + toUnit;
        }

        const UnitDef & from = fromIter->second;
        const UnitDef & to   = toIter->second;

        std::string result;

        // Do any necessary unit conversion
        if(fromIter->second.factor != 0.0 && toIter->second.factor != 0.0)
        {
            val = (T)((double)val / from.factor * to.factor);
        }

        // Do unit formatting
        if(!to.format.empty())
        {
            char str[100];
            snprintf(str, sizeof(str), to.format.c_str(), val);

            result = str;
        }
        else
        {
            std::stringstream str;
            str << (sizeof(T) == 1 ? (int)val : val); // workaround for stream treating uint8_t like a character
            result = str.str();
        }

        // Do output name remapping
        if(!to.outputMap.empty()) {
            const EnumDefMap::const_iterator it = to.outputMap.find(result);
            if(it != to.outputMap.end()) result = it->second;
        }

        return result;
    }
};

typedef std::map<std::string, UnitClassDef> UnitClassDefMap;

#endif

#include <iostream>
#include <sstream>
#include <cstdio>
#include "UnitClassDef.h"

/*
std::string UnitClassDef::getConversion(const std::string & fromUnit, const std::string & toUnit, double val) const
{
    const UnitDefMap::const_iterator fromIter = units.find(fromUnit);
    const UnitDefMap::const_iterator toIter = units.find(toUnit);

    if(fromIter == units.end() || toIter == units.end())
    {
        return "Invalid conversion: (" + name + ") " + fromUnit + "->" + toUnit;
    }

    double newVal = val / fromIter->second.factor * toIter->second.factor;

    std::cout << "Got new val: " << newVal << std::endl;

    if(!toIter->second.format.empty())
    {
        char str[100];
        snprintf(str, sizeof(str), toIter->second.format.c_str(), newVal);

        return std::string(str);
    }
    else
    {
        std::stringstream str;
        str << newVal;
        return str.str();
    }
}
*/

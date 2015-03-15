#ifndef DATUMIDENTIFIER_H
#define DATUMIDENTIFIER_H

#include <stdint.h>
#include <iostream>

class DatumIdentifier
{
private:
    uint16_t site;
    uint16_t app;
    uint16_t entity;

public:
    DatumIdentifier();
    bool operator==(const DatumIdentifier rhs) const;
    bool operator!=(const DatumIdentifier rhs) const;
    void setSite(uint16_t s);
    void setApp(uint16_t a);
    void setEntity(uint16_t e);
    uint16_t getSite();
    uint16_t getApp();
    uint16_t getEntity();
    uint32_t getDatumId();

    std::string getStringRepresentation() const;
    static DatumIdentifier fromStringRepresentation(std::string rep);
};

#endif // DATUMIDENTIFIER_H

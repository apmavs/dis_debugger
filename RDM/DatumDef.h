#ifndef DATUMDEF_H
#define DATUMDEF_H

#include "DatumInfo.h"
#include "DatumIdentifier.h"
#include "DatumDefId.h"

#include "KDIS/PDU/Header.h"

#include <QByteArray>
#include <vector>
#include <stdint.h>


class SenderId
{
public:
    uint16_t site;
    uint16_t app;
    uint16_t entity;

    SenderId() :
        site(0),
        app(0),
        entity(0)
    {}

    SenderId(uint16_t s, uint16_t a, uint16_t e)
    {
        site   = s;
        app    = a;
        entity = e;
    }
};

class DatumDef
{
protected:
    uint32_t length; // bytes
    uint32_t offset; // bytes
    std::string byte_order;
    std::string type;
    std::string unit;
    std::string unit_class;
    std::string name;
    std::string category;
    std::string description;
    DatumValue* minimum;
    bool has_minimum;
    DatumValue* maximum;
    bool has_maximum;
    std::string enum_type;

    DatumDef();
    void setDatumInfoId(const KDIS::PDU::Header* pdu, DatumInfo* datum);
    DatumDef & operator=(const DatumDef& copyVal);
    DatumDef(const DatumDef& copyVal);

public:
    virtual ~DatumDef();
    DatumDefId definition_id;

    static SenderId getSender(const KDIS::PDU::Header* pdu);

    void setDefinitionId(DatumDefId id);
    void setLength(uint32_t l); // in bytes
    void setOffset(uint32_t o); // in bytes
    void setByteOrder(std::string o);
    void setType(std::string t);
    void setUnit(std::string u);
    void setUnitClass(std::string u);
    void setName(std::string n);
    void setCategory(std::string c);
    void setDescription(std::string d);
    void setMin(std::string minVal, std::string type);
    void setMax(std::string maxVal, std::string type);
    void setEnumType(std::string enumType);

    virtual void getDatums(double timestamp,
                           const KDIS::PDU::Header* pdu,
                           const unsigned char* data,
                           uint32_t size, std::vector<DatumInfo*>* datums) = 0;
};

#endif // DATUMDEF_H

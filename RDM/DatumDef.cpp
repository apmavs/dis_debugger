#include "DatumDef.h"
#include <iostream>

#include "KDIS/DataTypes/EntityIdentifier.h"
#include "KDIS/PDU/Header.h"

DatumDef::DatumDef()
{
    length      = 0;
    offset      = 0;
    type        = "";
    unit        = "";
    unit_class  = "";
    name        = "";
    category    = "";
    description = "";
    minimum     = "";
    maximum     = "";
    my_id       = generateId();
}

DatumDef::~DatumDef()
{
}

void DatumDef::setDatumInfoId(KDIS::PDU::Header* pdu, DatumInfo* datum)
{
    uint16_t site   = 0;
    uint16_t app    = 0;
    uint16_t entity = 0;

    KDIS::KUINT16 headerSize = KDIS::PDU::Header::HEADER6_PDU_SIZE;
    KDIS::KUINT16 entityIdSize = KDIS::DATA_TYPE::EntityIdentifier::ENTITY_IDENTIFER_SIZE;
    KDIS::KUINT16 minSize = headerSize + entityIdSize;
    if(pdu->GetPDULength() >= minSize)
    {
        const KDIS::KOCTET *rawData = pdu->Encode().GetBufferPtr();
        rawData += headerSize; // Move past header
        site = *((uint16_t *)rawData);
        rawData += 2;
        app = *((uint16_t *)rawData);
        rawData += 2;
        entity = *((uint16_t *)rawData);
    }

    DatumIdentifier id;
    id.setSite(site);
    id.setApp(app);
    id.setEntity(entity);
    id.setDatumId(my_id);
    datum->setId(id);
}

// Assign a different ID to each datum definition
uint32_t DatumDef::generateId()
{
    static uint32_t lastId = 0;
    lastId++;
    return lastId;
}

void DatumDef::setDefinitionId(DatumDefId id)
{
    definition_id = id;
}

void DatumDef::setLength(uint32_t l)
{
    length = l;
}

void DatumDef::setOffset(uint32_t o)
{
    offset = o;
}

void DatumDef::setByteOrder(std::string o)
{
    byte_order = o;
}

void DatumDef::setType(std::string t)
{
    type = t;
}

void DatumDef::setUnit(std::string u)
{
    unit = u;
}

void DatumDef::setUnitClass(std::string u)
{
    unit_class = u;
}

void DatumDef::setName(std::string n)
{
    name = n;
}

void DatumDef::setCategory(std::string c)
{
    category = c;
}

void DatumDef::setDescription(std::string d)
{
    description = d;
}

void DatumDef::setMin(QByteArray m)
{
    minimum = m;
}

void DatumDef::setMax(QByteArray m)
{
    maximum = m;
}

void DatumDef::setEnumType(std::string enumType)
{
    enum_type = enumType;
}


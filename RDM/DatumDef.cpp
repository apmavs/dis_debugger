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

DatumDef::~DatumDef() {}

void DatumDef::setDatumInfoId(const KDIS::PDU::Header* pdu, DatumInfo* datum)
{
    SenderId sender = getSender(pdu);

    DatumIdentifier id;
    id.setSite(sender.site);
    id.setApp(sender.app);
    id.setEntity(sender.entity);
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

SenderId DatumDef::getSender(const KDIS::PDU::Header* pdu)
{
    SenderId sender;
    KDIS::KUINT16 headerSize = KDIS::PDU::Header::HEADER6_PDU_SIZE;
    KDIS::KUINT16 entityIdSize = KDIS::DATA_TYPE::EntityIdentifier::ENTITY_IDENTIFER_SIZE;
    KDIS::KUINT16 minSize = headerSize + entityIdSize;
    if(pdu->GetPDULength() >= minSize)
    {
        KDIS::KDataStream pduStream = pdu->Encode();
        uint16_t bufSize = pduStream.GetBufferSize();
        unsigned char* rawData = new unsigned char[bufSize];
        pduStream.CopyIntoBuffer((KDIS::KOCTET*)rawData, bufSize);

        // Create new pointer to parse out sender info
        unsigned char* pos = rawData + headerSize; // Move pass header
        sender.site = __builtin_bswap16(*((uint16_t*)pos));
        pos += 2;
        sender.app = __builtin_bswap16(*((uint16_t*)pos));
        pos += 2;
        sender.entity = __builtin_bswap16(*((uint16_t*)pos));

        delete [] rawData;
    }

    return sender;
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


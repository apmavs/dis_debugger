#include "DatumDef.h"

#include "KDIS/DataTypes/EntityIdentifier.h"
#include "KDIS/PDU/Header.h"

void DatumDef::setEntity(KDIS::PDU::Header pdu, DatumIdentifier id)
{
    uint16_t site   = 0;
    uint16_t app    = 0;
    uint16_t entity = 0;

    KDIS::KUINT16 headerSize = KDIS::PDU::Header::HEADER6_PDU_SIZE;
    KDIS::KUINT16 entityIdSize = KDIS::DATA_TYPE::EntityIdentifier::ENTITY_IDENTIFER_SIZE;
    KDIS::KUINT16 minSize = headerSize + entityIdSize;
    if(pdu.GetPDULength() >= minSize)
    {
        const KDIS::KOCTET *rawData = pdu.Encode().GetBufferPtr();
        rawData += headerSize; // Move past header
        site = *((uint16_t *)rawData);
        rawData += 2;
        app = *((uint16_t *)rawData);
        rawData += 2;
        entity = *((uint16_t *)rawData);
    }

    id.setSite(site);
    id.setApp(app);
    id.setEntity(entity);
}

void DatumDef::setLength(uint32_t l)
{
    length = l;
}

void DatumDef::setOffset(uint32_t o)
{
    offset = o;
}

void DatumDef::setType(std::string t)
{
    type = t;
}

void DatumDef::setUnit(std::string u)
{
    unit = u;
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

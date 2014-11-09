#include "BaseDef.h"

#include "KDIS/PDU/Entity_Info_Interaction/Entity_State_PDU.h"
#include <iostream>

BaseDef::BaseDef()
{
}

BaseDef::~BaseDef()
{
}

void BaseDef::getDatums(KDIS::PDU::Header* pdu, uint32_t size, std::vector<DatumInfo*>* datums)
{
    uint32_t minSize = offset + length;
    if(size >= minSize)
    {
        unsigned char* pos = (unsigned char *)pdu->Encode().GetBufferPtr();
        pos += offset;
        QByteArray value;
        for(uint32_t byteNum = 0; byteNum < length; byteNum++)
        {
            // TODO: Make use of byte_order
            value.append(pos[byteNum]);
        }
        DatumInfo* newDatum;
        newDatum = DatumInfo::createDatum(type, value);
        newDatum->setCategory(category);
        newDatum->setDescription(description);
        newDatum->setName(name);
        newDatum->setUnit(unit);
        newDatum->setUnitClass(unit_class);
        setDatumInfoId(pdu, newDatum);
        datums->push_back(newDatum);
    }
    else
    {
        std::cerr << "BaseDef:Error PDU size is less than my datum:" << name <<
                     std::endl;
    }
}

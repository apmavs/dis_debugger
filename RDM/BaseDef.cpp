#include "BaseDef.h"

#include "KDIS/PDU/Entity_Info_Interaction/Entity_State_PDU.h"
#include <iostream>

BaseDef::BaseDef()
{
}

BaseDef::~BaseDef()
{
}

void BaseDef::getDatums(double timestamp, const KDIS::PDU::Header* pdu,
                        const unsigned char* data,
                        uint32_t size, std::vector<DatumInfo*>* datums)
{
    uint32_t minSize = offset + length;
    if(size >= minSize)
    {
        data += offset;
        QByteArray value;

        if(byte_order == "network")
        {
            for(int32_t byteNum = length - 1; byteNum >= 0; byteNum--)
            {
                value.append(data[byteNum]);
            }
        }
        else
        {
            for(uint32_t byteNum = 0; byteNum < length; byteNum++)
            {
                value.append(data[byteNum]);
            }
        }
        DatumInfo* newDatum;
        newDatum = DatumInfo::createDatum(timestamp, type, value);
        newDatum->setCategory(category);
        newDatum->setDescription(description);
        newDatum->setName(name);
        newDatum->setUnit(unit);
        newDatum->setUnitClass(unit_class);
        if(has_minimum) newDatum->setMinimum(minimum);
        if(has_maximum) newDatum->setMaximum(maximum);
        setDatumInfoId(pdu, newDatum);
        datums->push_back(newDatum);
    }
    else
    {
        std::cerr << "BaseDef:Error PDU size is less than my datum:" << name <<
                     std::endl;
    }
}

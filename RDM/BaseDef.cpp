#include "BaseDef.h"

#include <iostream>

BaseDef::BaseDef()
{
    initializeMembers();
}

std::vector<DatumInfo> BaseDef::getDatums(KDIS::PDU::Header* pdu)
{
    std::vector<DatumInfo> datums;
    uint32_t minSize = offset + length;
    if(pdu->GetPDULength() > minSize)
    {
        unsigned char* pos = (unsigned char *)pdu;
        pos += offset;
        QByteArray value;
        for(uint32_t byteNum = 0; byteNum < length; byteNum++)
        {
            // TODO: Make use of byte_order
            value.append(pos[byteNum]);
        }
        DatumInfo newDatum;
        newDatum.addValue(DatumValue(-1.0, value)); // Let controller set times
        newDatum.setCategory(category);
        newDatum.setDescription(description);
        newDatum.setName(name);
        newDatum.setType(type);
        newDatum.setUnit(unit);
        newDatum.setUnitClass(unit_class);
        datums.push_back(newDatum);
    }
    else
    {
        std::cerr << "BaseDef:Error PDU size is less than my datum:" << name <<
                     std::endl;
    }

    return datums;
}

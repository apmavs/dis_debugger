#include "PduDef.h"

PduDef::PduDef()
{
    initializeMembers();
}

PduDef::~PduDef()
{
    std::vector<DatumDef*>::iterator it;
    for(it = base_defs.begin(); it != base_defs.end(); it++)
    {
        delete (*it);
    }
}

void PduDef::add(DatumDef* d)
{
    base_defs.push_back(d);
}

void PduDef::getDatums(KDIS::PDU::Header* pdu, uint32_t size, std::vector<DatumInfo>* datums)
{
    std::vector<DatumDef*>::iterator it;
    for(it = base_defs.begin(); it != base_defs.end(); it++)
    {
        (*it)->getDatums(pdu, size, datums);
    }

    std::vector<DatumInfo>::iterator datumIt;
    for(datumIt = datums->begin(); datumIt != datums->end(); datumIt++)
    {
        setDatumInfoId(pdu, &(*datumIt));
    }
}

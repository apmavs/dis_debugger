#include "PduDef.h"

PduDef::PduDef()
{
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

void PduDef::getDatums(const KDIS::PDU::Header* pdu,
                       const unsigned char* data,
                       uint32_t size, std::vector<DatumInfo*>* datums)
{
    std::vector<DatumDef*>::iterator it;
    for(it = base_defs.begin(); it != base_defs.end(); it++)
    {
        (*it)->getDatums(pdu, data, size, datums);
    }
}

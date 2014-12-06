#ifndef PDUDEF_H
#define PDUDEF_H

#include "DatumDef.h"
#include <vector>

class PduDef : public DatumDef
{
protected:
    std::vector<DatumDef*> base_defs;

public:
    PduDef();
    virtual ~PduDef();
    void add(DatumDef* d);
    virtual void getDatums(KDIS::PDU::Header* pdu, unsigned char* data,
                        uint32_t size, std::vector<DatumInfo*>* datums);
};

#endif // PDUDEF_H

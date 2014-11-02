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
    void add(DatumDef* d);
    virtual std::vector<DatumInfo> getDatums(KDIS::PDU::Header* pdu);
};

#endif // PDUDEF_H

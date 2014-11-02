#ifndef COMPLEXDEF_H
#define COMPLEXDEF_H

#include "DatumDef.h"
#include "PduDef.h"

#include <map>
#include <vector>

class ComplexDef : public PduDef
{
protected:
    std::map<uint32_t, std::vector<DatumDef*>*> fixed_defs;
    std::map<uint32_t, std::vector<DatumDef*>*> variable_defs;

public:
    ComplexDef();
    void add(DatumDef* d);
    virtual std::vector<DatumInfo> getDatums(KDIS::PDU::Header* pdu);
};

#endif // COMPLEXDEF_H

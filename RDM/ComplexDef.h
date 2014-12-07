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

    void parseFixedDatum(const KDIS::PDU::Header* pdu,
                         const unsigned char* fixedDatum,
                         std::vector<DatumInfo*>* datums);
    void parseVariableDatums(const KDIS::PDU::Header* pdu,
                             const unsigned char* varDatum,
                             std::vector<DatumInfo*>* datums);

public:
    ComplexDef();
    virtual ~ComplexDef();
    void add(DatumDef* d);
    virtual void getDatums(const KDIS::PDU::Header* pdu,
                           const unsigned char* data,
                           uint32_t size, std::vector<DatumInfo*>* datums);
};

#endif // COMPLEXDEF_H

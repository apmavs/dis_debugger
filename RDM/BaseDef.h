#ifndef BASEDEF_H
#define BASEDEF_H

#include "DatumDef.h"

class BaseDef : public DatumDef
{
public:
    BaseDef();
    virtual ~BaseDef();

    virtual void getDatums(KDIS::PDU::Header* pdu, uint32_t size, std::vector<DatumInfo>* datums);
};

#endif // BASEDEF_H

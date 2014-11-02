#ifndef BASEDEF_H
#define BASEDEF_H

#include "DatumDef.h"

class BaseDef : public DatumDef
{
public:
    BaseDef();

    virtual std::vector<DatumInfo> getDatums(KDIS::PDU::Header* pdu);
};

#endif // BASEDEF_H

#include "PduDef.h"

PduDef::PduDef()
{
    initializeMembers();
}

void PduDef::add(DatumDef* d)
{
    base_defs.push_back(d);
}

std::vector<DatumInfo> PduDef::getDatums(KDIS::PDU::Header* pdu)
{
    std::vector<DatumInfo> datums;
    std::vector<DatumDef*>::iterator it;
   for(it = base_defs.begin(); it != base_defs.end(); it++)
   {
       std::vector<DatumInfo> subdatums = (*it)->getDatums(pdu);
       std::vector<DatumInfo>::iterator itSub;
       for(itSub = subdatums.begin(); itSub != subdatums.end(); itSub++)
       {
           datums.push_back(*itSub);
       }
   }

   return datums;
}

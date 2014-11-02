#include "ComplexDef.h"

ComplexDef::ComplexDef()
{
    initializeMembers();
}

void ComplexDef::add(DatumDef* d)
{
    DatumDefId defId = d->getDefinitionId();
    uint32_t fixedType    = defId.getFixedType();
    uint32_t variableType = defId.getVariableType();
    if((fixedType == 0) && (variableType == 0))
    {
        base_defs.push_back(d);
    }
    else
    {
        if(fixedType != 0)
        {
            std::vector<DatumDef*>* fvect;
            if(fixed_defs.count(fixedType))
            {
                fvect = fixed_defs[fixedType];
            }
            else
            {
                fvect = new std::vector<DatumDef*>();
            }
            fvect->push_back(d);
        }
        if(variableType != 0)
        {
            std::vector<DatumDef*>* vvect;
            if(variable_defs.count(variableType))
            {
                vvect = variable_defs[variableType];
            }
            else
            {
                vvect = new std::vector<DatumDef*>();
            }
            vvect->push_back(d);
        }
    }
}

std::vector<DatumInfo> ComplexDef::getDatums(KDIS::PDU::Header* pdu)
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
   // TODO: fill this in correctly by checking how many datums exist
   // and parsing out the id
//   for(it = fixed_defs.begin(); it != fixed_defs.end(); it++)
//   {
//       std::vector<DatumInfo> subdatums = (*it)->getDatums(pdu);
//       std::vector<DatumInfo>::iterator itSub;
//       for(itSub = subdatums.begin(); itSub != subdatums.end(); itSub++)
//       {
//           datums.push_back(*itSub);
//       }
//   }
//   for(it = variable_defs.begin(); it != variable_defs.end(); it++)
//   {
//       std::vector<DatumInfo> subdatums = (*it)->getDatums(pdu);
//       std::vector<DatumInfo>::iterator itSub;
//       for(itSub = subdatums.begin(); itSub != subdatums.end(); itSub++)
//       {
//           datums.push_back(*itSub);
//       }
//   }

   return datums;
}

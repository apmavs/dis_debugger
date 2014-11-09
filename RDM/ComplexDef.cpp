#include "ComplexDef.h"


ComplexDef::ComplexDef()
{
}

ComplexDef::~ComplexDef()
{
    std::map<uint32_t, std::vector<DatumDef*>*>::iterator it;
    for(it = fixed_defs.begin(); it != fixed_defs.end(); it++)
    {
        std::vector<DatumDef*>* vecPtr = it->second;
        std::vector<DatumDef*>::iterator itDef;
        for(itDef = vecPtr->begin(); itDef != vecPtr->end(); itDef++)
        {
            delete (*itDef);
        }
        delete vecPtr;
    }
    for(it = variable_defs.begin(); it != variable_defs.end(); it++)
    {
        std::vector<DatumDef*>* vecPtr = it->second;
        std::vector<DatumDef*>::iterator itDef;
        for(itDef = vecPtr->begin(); itDef != vecPtr->end(); itDef++)
        {
            delete (*itDef);
        }
        delete vecPtr;
    }
}

void ComplexDef::add(DatumDef* d)
{
    uint32_t fixedType    = d->definition_id.getFixedType();
    uint32_t variableType = d->definition_id.getVariableType();
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
                fixed_defs[fixedType] = fvect;
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
                variable_defs[variableType] = vvect;
            }
            vvect->push_back(d);
        }
    }
}

void ComplexDef::getDatums(KDIS::PDU::Header* pdu, uint32_t size, std::vector<DatumInfo*>* datums)
{
    std::vector<DatumDef*>::iterator it;
    for(it = base_defs.begin(); it != base_defs.end(); it++)
    {
        (*it)->getDatums(pdu, size, datums);
    }

    if(((pdu->GetPDUType() == KDIS::DATA_TYPE::ENUMS::Set_Data_PDU_Type) ||
       (pdu->GetPDUType() == KDIS::DATA_TYPE::ENUMS::Data_PDU_Type)) &&
       (size >= 40))
    {
        unsigned char* data = (unsigned char*)pdu;
        data += 32; // offset to num fix datum records
        uint32_t numFixed = *((uint32_t*)data);
        data += 4; // offset to num variable datum records
        uint32_t numVar = *((uint32_t*)data);

        data += 4; // offset to first fixed/variable datum
        for(uint32_t fixedIdx = 0; fixedIdx < numFixed; fixedIdx++)
        {
            parseFixedDatum(data, datums);
            data += 8;
        }
        for(uint32_t varIdx = 0; varIdx < numVar; varIdx++)
        {
            parseVariableDatums(data, datums);
            uint32_t varSize = (*((uint32_t*)(data+4))) / 8;
            data += varSize;
        }
    }

    std::vector<DatumInfo*>::iterator datumIt;
    for(datumIt = datums->begin(); datumIt != datums->end(); datumIt++)
    {
        setDatumInfoId(pdu, *datumIt);
    }
}

// Parse through each fixed datum and add it to the vector
void ComplexDef::parseFixedDatum(unsigned char* fixedDatum, std::vector<DatumInfo*>* datums)
{
    uint32_t fixedId = *((uint32_t*)fixedDatum);
    if(fixed_defs.count(fixedId))
    {
        std::vector<DatumDef*>* defs = fixed_defs[fixedId];
        std::vector<DatumDef*>::iterator it;
        for(it = defs->begin(); it != defs->end(); it++)
        {
            unsigned char* datumPtr = fixedDatum + 4;
            (*it)->getDatums((KDIS::PDU::Header*)datumPtr, 4, datums);
        }
    }
}

// Parse through each variable datum and add it to the vector
void ComplexDef::parseVariableDatums(unsigned char* varDatum, std::vector<DatumInfo*>* datums)
{
    uint32_t varId = *((uint32_t*)varDatum);
    if(variable_defs.count(varId))
    {
        uint32_t size = (*((uint32_t*)(varDatum + 4))) / 8;
        std::vector<DatumDef*>* defs = variable_defs[varId];
        std::vector<DatumDef*>::iterator it;
        for(it = defs->begin(); it != defs->end(); it++)
        {
            unsigned char* datumPtr = varDatum + 8;
            (*it)->getDatums((KDIS::PDU::Header*)datumPtr, size, datums);
        }
    }
}

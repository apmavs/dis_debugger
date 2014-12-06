#include "ComplexDef.h"

#include <iostream>


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

void ComplexDef::getDatums(KDIS::PDU::Header* pdu, unsigned char* data,
                        uint32_t size, std::vector<DatumInfo*>* datums)
{
    std::vector<DatumDef*>::iterator it;
    for(it = base_defs.begin(); it != base_defs.end(); it++)
    {
        (*it)->getDatums(pdu, data, size, datums);
    }

    if(((pdu->GetPDUType() == KDIS::DATA_TYPE::ENUMS::Set_Data_PDU_Type) ||
       (pdu->GetPDUType() == KDIS::DATA_TYPE::ENUMS::Data_PDU_Type)) &&
       (size >= 40))
    {
        std::cout << "Size is:" << pdu->Encode().GetBufferSize() << std::endl;
        data += 32; // offset to num fix datum records
        uint32_t numFixed = __builtin_bswap32(*((uint32_t*)data));
        data += 4; // offset to num variable datum records
        uint32_t numVar = __builtin_bswap32(*((uint32_t*)data));

        uint32_t minSize = 40 + (numFixed*4) + (numVar*4);
        if(size >= minSize)
        {
            std::cout << "Fixed size is:" << numFixed << std::endl;
            std::cout << "Var size is:" << numVar << std::endl;
            data += 4; // offset to first fixed/variable datum
            for(uint32_t fixedIdx = 0; fixedIdx < numFixed; fixedIdx++)
            {
                parseFixedDatum(pdu, data, datums);
                data += 8;
            }
            for(uint32_t varIdx = 0; varIdx < numVar; varIdx++)
            {
                parseVariableDatums(pdu, data, datums);
                uint32_t varSize = (*((uint32_t*)(data+4))) / 8;
                std::cout << "Var piece size is:" << varSize << std::endl;
                data += varSize;
            }
        }
        else
        {
            std::cerr << "ComplexDef::getDatums ERROR:";
            std::cerr << "PDU has size " << size << " bytes, but contents indicate ";
            std::cerr << "a size of at least " << minSize << " bytes" << std::endl;
        }
    }
}

// Parse through each fixed datum and add it to the vector
void ComplexDef::parseFixedDatum(KDIS::PDU::Header* pdu,
                                 unsigned char* fixedDatum,
                                 std::vector<DatumInfo*>* datums)
{
    uint32_t fixedId = __builtin_bswap32(*((uint32_t*)fixedDatum));
    std::cout << "Parsing fixed datum with fixedId:" << fixedId << std::endl;
    if(fixed_defs.count(fixedId))
    {
        std::cout << "Parsing fixed_defs list with fixedId:" << fixedId << std::endl;
        std::vector<DatumDef*>* defs = fixed_defs[fixedId];
        std::vector<DatumDef*>::iterator it;
        for(it = defs->begin(); it != defs->end(); it++)
        {
            std::cout << "Getting datums with fixedId:" << fixedId << std::endl;
            unsigned char* datumPtr = fixedDatum + 4;
            (*it)->getDatums(pdu, datumPtr, 4, datums);
        }
    }
}

// Parse through each variable datum and add it to the vector
void ComplexDef::parseVariableDatums(KDIS::PDU::Header* pdu,
                             unsigned char* varDatum,
                             std::vector<DatumInfo*>* datums)
{
    uint32_t varId = __builtin_bswap32(*((uint32_t*)varDatum));
    if(variable_defs.count(varId))
    {
        uint32_t size = __builtin_bswap32((*((uint32_t*)(varDatum + 4)))) / 8;
        std::vector<DatumDef*>* defs = variable_defs[varId];
        std::vector<DatumDef*>::iterator it;
        for(it = defs->begin(); it != defs->end(); it++)
        {
            unsigned char* datumPtr = varDatum + 8;
            (*it)->getDatums(pdu, datumPtr, size, datums);
        }
    }
}

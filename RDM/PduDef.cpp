#include "PduDef.h"

#include <iostream>


PduDef::PduDef() : DatumDef()
{
}

PduDef::~PduDef()
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

void PduDef::add(DatumDef* d)
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

void PduDef::getDatums(double timestamp, const KDIS::PDU::Header* pdu,
                       const unsigned char* data,
                       uint32_t size, std::vector<DatumInfo*>* datums)
{
    // Snap the starting address of data before we start parsing it
    const unsigned char* dataStart = data;

    std::vector<DatumDef*>::iterator it;
    for(it = base_defs.begin(); it != base_defs.end(); it++)
    {
        (*it)->getDatums(timestamp, pdu, data, size, datums);
    }

    if(((pdu->GetPDUType() == KDIS::DATA_TYPE::ENUMS::Set_Data_PDU_Type) ||
       (pdu->GetPDUType() == KDIS::DATA_TYPE::ENUMS::Data_PDU_Type)) &&
       (size >= 40))
    {
        data += 32; // offset to num fix datum records
        uint32_t numFixed = __builtin_bswap32(*((uint32_t*)data));
        data += 4; // offset to num variable datum records
        uint32_t numVar = __builtin_bswap32(*((uint32_t*)data));

        uint32_t minSize = 40 + (numFixed*4) + (numVar*4);
        if(size >= minSize)
        {
            data += 4; // offset to first fixed/variable datum
            for(uint32_t fixedIdx = 0; fixedIdx < numFixed; fixedIdx++)
            {
                parseFixedDatum(timestamp, pdu, data, datums);
                data += 8;
            }
            for(uint32_t varIdx = 0; varIdx < numVar; varIdx++)
            {
                const unsigned char* sizePtr = data + 4;
                uint32_t varSize = __builtin_bswap32(*((uint32_t*)(sizePtr))) / 8;
                varSize = __builtin_bswap32(*((uint32_t*)(sizePtr))) / 8;

                // Ensure size of entire PDU is big enough to hold
                // what the variable datum says it's size is
                uint32_t sizeSoFar = (data - dataStart) + varSize;
                if(size >= sizeSoFar)
                {
                    parseVariableDatums(timestamp, pdu, data, datums);
                }
                else
                {
                    std::cerr << "PduDef Error: Rxed PDU requires at least";
                    std::cerr << " " << sizeSoFar << " bytes, but PDU is only ";
                    std::cerr << size << " bytes large." << std::endl;
                }

                varSize = __builtin_bswap32(*((uint32_t*)(sizePtr))) / 8;
                data += varSize;
            }
        }
        else
        {
            std::cerr << "PduDef::getDatums ERROR:";
            std::cerr << "PDU has size " << size << " bytes, but contents indicate ";
            std::cerr << "a size of at least " << minSize << " bytes" << std::endl;
        }
    }
}

// Parse through each fixed datum and add it to the vector
void PduDef::parseFixedDatum(double timestamp,
                             const KDIS::PDU::Header* pdu,
                             const unsigned char* fixedDatum,
                             std::vector<DatumInfo*>* datums)
{
    uint32_t fixedId = __builtin_bswap32(*((uint32_t*)fixedDatum));
    if(fixed_defs.count(fixedId))
    {
        std::vector<DatumDef*>* defs = fixed_defs[fixedId];
        std::vector<DatumDef*>::iterator it;
        for(it = defs->begin(); it != defs->end(); it++)
        {
            const unsigned char* datumPtr = fixedDatum + 4;
            (*it)->getDatums(timestamp, pdu, datumPtr, 4, datums);
        }
    }
}

// Parse through each variable datum and add it to the vector
void PduDef::parseVariableDatums(double timestamp,
                                 const KDIS::PDU::Header* pdu,
                                 const unsigned char* varDatum,
                                 std::vector<DatumInfo*>* datums)
{
    uint32_t varId = __builtin_bswap32(*((uint32_t*)varDatum));
    if(variable_defs.count(varId))
    {
        varDatum += 4; // Skip to size
        uint32_t size = __builtin_bswap32(*((uint32_t*)(varDatum))) / 8;
        varDatum += 4; // Skip to value
        std::vector<DatumDef*>* defs = variable_defs[varId];
        std::vector<DatumDef*>::iterator it;
        for(it = defs->begin(); it != defs->end(); it++)
        {
            (*it)->getDatums(timestamp, pdu, varDatum, size, datums);
        }
    }
}

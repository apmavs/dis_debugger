#include "PduDeconstructor.h"
#include "VehicleMetadataLoader.h"

PduDeconstructor::PduDeconstructor()
{
}

bool PduDeconstructor::loadXml(std::string filename)
{
    bool success = true;

    VehicleMetadataLoader* loader = new VehicleMetadataLoader(filename);

    mutex.lock();
    definitions = loader->getDefinitions();
    mutex.unlock();

    if(loader->errorDetected())
        success = false;
    delete loader;

    return success;
}

std::vector<DatumInfo> PduDeconstructor::deconstruct(KDIS::PDU::Header* pdu)
{
    std::vector<DatumInfo> retVal;
    uint32_t size = pdu->GetPDULength();
    KDIS::DATA_TYPE::ENUMS::PDUType type = pdu->GetPDUType();
    if((type == KDIS::DATA_TYPE::ENUMS::Entity_State_PDU_Type) &&
       (definitions.count(KDIS::DATA_TYPE::ENUMS::Entity_State_PDU_Type) > 0))
    {
        definitions[KDIS::DATA_TYPE::ENUMS::Entity_State_PDU_Type]->getDatums(pdu, size, &retVal);
    }
    else if((type == KDIS::DATA_TYPE::ENUMS::Data_PDU_Type) &&
       (definitions.count(KDIS::DATA_TYPE::ENUMS::Data_PDU_Type) > 0))
    {
        definitions[KDIS::DATA_TYPE::ENUMS::Data_PDU_Type]->getDatums(pdu, size, &retVal);
    }
    else if((type == KDIS::DATA_TYPE::ENUMS::Set_Data_PDU_Type) &&
       (definitions.count(KDIS::DATA_TYPE::ENUMS::Set_Data_PDU_Type) > 0))
    {
        definitions[KDIS::DATA_TYPE::ENUMS::Set_Data_PDU_Type]->getDatums(pdu, size, &retVal);
    }

    return retVal;
}

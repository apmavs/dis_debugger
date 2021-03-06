#include "PduDeconstructor.h"

#include <iostream>
#include <KDIS/PDU/Entity_Info_Interaction/Entity_State_PDU.h>


std::map<KDIS::DATA_TYPE::EntityIdentifier, std::string> PduDeconstructor::entity_markings;

PduDeconstructor::PduDeconstructor()
{
}

bool PduDeconstructor::loadXml(std::string filename)
{
    bool success = true;

    VehicleMetadataLoader* loader = new VehicleMetadataLoader(filename);

    mutex.lock();
    // Destroy current definitions if they exist
    std::map<uint8_t, PduDef*>::iterator it;
    for(it = definitions.begin(); it != definitions.end(); it++)
    {
        PduDef* def = it->second;
        delete def;
    }

    definitions.clear();
    unitClasses.clear();

    loader->load(definitions, unitClasses);
    mutex.unlock();

    if(loader->errorDetected())
        success = false;
    delete loader;

    return success;
}

std::vector<DatumInfo*> PduDeconstructor::deconstruct(double timestamp, KDIS::PDU::Header* pdu)
{
    std::vector<DatumInfo*> retVal;
    KDIS::DATA_TYPE::ENUMS::PDUType type = pdu->GetPDUType();

    // If entity state pdu, save off the entity's name
    if(type == KDIS::DATA_TYPE::ENUMS::Entity_State_PDU_Type)
    {
        try
        {
            KDIS::PDU::Entity_State_PDU* entityPdu = (KDIS::PDU::Entity_State_PDU*)pdu;
            KDIS::DATA_TYPE::EntityIdentifier id = entityPdu->GetEntityIdentifier();
            std::string marking = entityPdu->GetEntityMarking().GetEntityMarkingString();
            entity_markings[id] = marking;
        }
        catch(std::exception &e)
        {
            std::cerr << "PduDeconstructor:Error getting marking:" << e.what()
                     << std::endl;
        }
    }

    // Deconstruct datums from PDU
    if(definitions.count(type) > 0)
    {
        KDIS::KDataStream pduStream = pdu->Encode();
        uint16_t bufSize = pduStream.GetBufferSize();
        unsigned char* data = (unsigned char *)pduStream.GetBufferPtr();
        definitions[type]->getDatums(timestamp, pdu, data, bufSize, &retVal);
    }

    return retVal;
}

std::string PduDeconstructor::getEntityMarking(uint16_t site, uint16_t app, uint16_t entity)
{
    std::string retVal = "";
    KDIS::DATA_TYPE::EntityIdentifier id(site, app, entity);
    if(entity_markings.count(id))
        retVal = entity_markings[id];

    return retVal;
}

std::string PduDeconstructor::getEntityIdentifier(uint16_t site, uint16_t app, uint16_t entity)
{
    std::string marking = getEntityMarking(site, app, entity);
    std::string entityIdentifier = marking + " " +
                                   QString("(%2:%3:%4)").arg(site)
                                                  .arg(app)
                                                  .arg(entity)
                                                  .toStdString();
    return entityIdentifier;
}

const UnitClassDef *PduDeconstructor::getUnitClassDef(const std::string & className) {
    UnitClassDefMap::const_iterator it = unitClasses.find(className);
    return it == unitClasses.end() ? NULL : &it->second;
}

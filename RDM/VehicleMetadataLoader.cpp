#include "VehicleMetadataLoader.h"

#include <QFile>
#include <iostream>

VehicleMetadataLoader::VehicleMetadataLoader(std::string filename)
{
    file_error_detected = false;

    // Open XML file
    QFile f(QString::fromStdString(filename));
    if(!f.open(QIODevice::ReadOnly))
    {
        std::cerr << "VehicleMetadataLoader: Unable to open: " << filename << std::endl;
        file_error_detected = true;
    }
    else
    {
        xml_doc.setContent(&f);
        f.close();
    }
}

VehicleMetadataLoader::~VehicleMetadataLoader()
{
}

bool VehicleMetadataLoader::errorDetected()
{
    return file_error_detected;
}

std::map<uint8_t, PduDef*> VehicleMetadataLoader::getDefinitions()
{
    std::map<uint8_t, PduDef*> pduDefs;

    // Parse contents of opened XML file
    QDomElement root = xml_doc.documentElement();
    while(!root.isNull())
    {
        if(root.tagName() == "DatumDefinitions")
        {
            createDatumDefinitions(root, &pduDefs);
        }
        root = root.nextSibling().toElement();
    }


    return pduDefs;
}


void VehicleMetadataLoader::createDatumDefinitions(QDomElement e,
                                          std::map<uint8_t, PduDef*>* defs)
{
    e = e.firstChild().toElement();
    while(!e.isNull())
    {
        QString source, id;
        source    = e.attribute("source", "UNKNOWN");
        id        = e.attribute("id", "UNKNOWN");

        if(source == "EntityState")
        {
            BaseDef* def = new BaseDef();
            populateDatumInfo(e, def);
            addDefToPduDef(def, KDIS::DATA_TYPE::ENUMS::Entity_State_PDU_Type, defs);
        }
        else if(source == "Other")
        {
            BaseDef* def = new BaseDef();
            populateDatumInfo(e, def);
            addDefToPduDef(def, KDIS::DATA_TYPE::ENUMS::Other_PDU_Type, defs);
        }
        else if(source == "Fire")
        {
            BaseDef* def = new BaseDef();
            populateDatumInfo(e, def);
            addDefToPduDef(def, KDIS::DATA_TYPE::ENUMS::Fire_PDU_Type, defs);
        }
        else if(source == "Detonation")
        {
            BaseDef* def = new BaseDef();
            populateDatumInfo(e, def);
            addDefToPduDef(def, KDIS::DATA_TYPE::ENUMS::Detonation_PDU_Type, defs);
        }
        else if(source == "Collision")
        {
            BaseDef* def = new BaseDef();
            populateDatumInfo(e, def);
            addDefToPduDef(def, KDIS::DATA_TYPE::ENUMS::Collision_PDU_Type, defs);
        }
        else if(source == "ServiceRequest")
        {
            BaseDef* def = new BaseDef();
            populateDatumInfo(e, def);
            addDefToPduDef(def, KDIS::DATA_TYPE::ENUMS::Service_Request_PDU_Type, defs);
        }
        else if(source == "ResupplyOffer")
        {
            BaseDef* def = new BaseDef();
            populateDatumInfo(e, def);
            addDefToPduDef(def, KDIS::DATA_TYPE::ENUMS::Resupply_Offer_PDU_Type, defs);
        }
        else if(source == "ResupplyReceived")
        {
            BaseDef* def = new BaseDef();
            populateDatumInfo(e, def);
            addDefToPduDef(def, KDIS::DATA_TYPE::ENUMS::Resupply_Received_PDU_Type, defs);
        }
        else if(source == "ResupplyCancel")
        {
            BaseDef* def = new BaseDef();
            populateDatumInfo(e, def);
            addDefToPduDef(def, KDIS::DATA_TYPE::ENUMS::Resupply_Cancel_PDU_Type, defs);
        }
        else if(source == "RepairComplete")
        {
            BaseDef* def = new BaseDef();
            populateDatumInfo(e, def);
            addDefToPduDef(def, KDIS::DATA_TYPE::ENUMS::Repair_Complete_PDU_Type, defs);
        }
        else if(source == "RepairResponse")
        {
            BaseDef* def = new BaseDef();
            populateDatumInfo(e, def);
            addDefToPduDef(def, KDIS::DATA_TYPE::ENUMS::Repair_Response_PDU_Type, defs);
        }
        else if(source == "CreateEntity")
        {
            BaseDef* def = new BaseDef();
            populateDatumInfo(e, def);
            addDefToPduDef(def, KDIS::DATA_TYPE::ENUMS::Create_Entity_PDU_Type, defs);
        }
        else if(source == "RemoveEntity")
        {
            BaseDef* def = new BaseDef();
            populateDatumInfo(e, def);
            addDefToPduDef(def, KDIS::DATA_TYPE::ENUMS::Remove_Entity_PDU_Type, defs);
        }
        else if(source == "StartResume")
        {
            BaseDef* def = new BaseDef();
            populateDatumInfo(e, def);
            addDefToPduDef(def, KDIS::DATA_TYPE::ENUMS::Start_Resume_PDU_Type, defs);
        }
        else if(source == "StopFreeze")
        {
            BaseDef* def = new BaseDef();
            populateDatumInfo(e, def);
            addDefToPduDef(def, KDIS::DATA_TYPE::ENUMS::Stop_Freeze_PDU_Type, defs);
        }
        else if(source == "Acknowledge")
        {
            BaseDef* def = new BaseDef();
            populateDatumInfo(e, def);
            addDefToPduDef(def, KDIS::DATA_TYPE::ENUMS::Acknowledge_PDU_Type, defs);
        }
        else if(source == "ActionRequest")
        {
            BaseDef* def = new BaseDef();
            populateDatumInfo(e, def);
            addDefToPduDef(def, KDIS::DATA_TYPE::ENUMS::Action_Request_PDU_Type, defs);
        }
        else if(source == "ActionResponse")
        {
            BaseDef* def = new BaseDef();
            populateDatumInfo(e, def);
            addDefToPduDef(def, KDIS::DATA_TYPE::ENUMS::Action_Response_PDU_Type, defs);
        }
        else if(source == "DataQuery")
        {
            BaseDef* def = new BaseDef();
            populateDatumInfo(e, def);
            addDefToPduDef(def, KDIS::DATA_TYPE::ENUMS::Data_Query_PDU_Type, defs);
        }
        else if(source == "EventReport")
        {
            BaseDef* def = new BaseDef();
            populateDatumInfo(e, def);
            addDefToPduDef(def, KDIS::DATA_TYPE::ENUMS::Event_Report_PDU_Type, defs);
        }
        else if(source == "Message")
        {
            BaseDef* def = new BaseDef();
            populateDatumInfo(e, def);
            addDefToPduDef(def, KDIS::DATA_TYPE::ENUMS::Message_PDU_Type, defs);
        }
        else if(source == "ElectromagneticEmission")
        {
            BaseDef* def = new BaseDef();
            populateDatumInfo(e, def);
            addDefToPduDef(def, KDIS::DATA_TYPE::ENUMS::Electromagnetic_Emission_PDU_Type, defs);
        }
        else if(source == "Designator")
        {
            BaseDef* def = new BaseDef();
            populateDatumInfo(e, def);
            addDefToPduDef(def, KDIS::DATA_TYPE::ENUMS::Designator_PDU_Type, defs);
        }
        else if(source == "Transmitter")
        {
            BaseDef* def = new BaseDef();
            populateDatumInfo(e, def);
            addDefToPduDef(def, KDIS::DATA_TYPE::ENUMS::Transmitter_PDU_Type, defs);
        }
        else if(source == "Signal")
        {
            BaseDef* def = new BaseDef();
            populateDatumInfo(e, def);
            addDefToPduDef(def, KDIS::DATA_TYPE::ENUMS::Signal_PDU_Type, defs);
        }
        else if(source == "Receiver")
        {
            BaseDef* def = new BaseDef();
            populateDatumInfo(e, def);
            addDefToPduDef(def, KDIS::DATA_TYPE::ENUMS::Receiver_PDU_Type, defs);
        }
        else if(source == "IFF_ATC_NAVAID")
        {
            BaseDef* def = new BaseDef();
            populateDatumInfo(e, def);
            addDefToPduDef(def, KDIS::DATA_TYPE::ENUMS::IFF_ATC_NAVAIDS_PDU_Type, defs);
        }
        else if(source == "UnderwaterAcoustic")
        {
            BaseDef* def = new BaseDef();
            populateDatumInfo(e, def);
            addDefToPduDef(def, KDIS::DATA_TYPE::ENUMS::UnderwaterAcoustic_PDU_Type, defs);
        }
        else if(source == "SupplementalEmission")
        {
            BaseDef* def = new BaseDef();
            populateDatumInfo(e, def);
            addDefToPduDef(def, KDIS::DATA_TYPE::ENUMS::SupplementalEmission_EntityState_PDU_Type, defs);
        }
        else if(source == "IntercomSignal")
        {
            BaseDef* def = new BaseDef();
            populateDatumInfo(e, def);
            addDefToPduDef(def, KDIS::DATA_TYPE::ENUMS::IntercomSignal_PDU_Type, defs);
        }
        else if(source == "IntercomControl")
        {
            BaseDef* def = new BaseDef();
            populateDatumInfo(e, def);
            addDefToPduDef(def, KDIS::DATA_TYPE::ENUMS::IntercomControl_PDU_Type, defs);
        }
        else if(source == "AggregateState")
        {
            BaseDef* def = new BaseDef();
            populateDatumInfo(e, def);
            addDefToPduDef(def, KDIS::DATA_TYPE::ENUMS::AggregateState_PDU_Type, defs);
        }
        else if(source == "IsGroupOf")
        {
            BaseDef* def = new BaseDef();
            populateDatumInfo(e, def);
            addDefToPduDef(def, KDIS::DATA_TYPE::ENUMS::IsGroupOf_PDU_Type, defs);
        }
        else if(source == "TransferControl")
        {
            BaseDef* def = new BaseDef();
            populateDatumInfo(e, def);
            addDefToPduDef(def, KDIS::DATA_TYPE::ENUMS::TransferControl_PDU_Type, defs);
        }
        else if(source == "IsPartOf")
        {
            BaseDef* def = new BaseDef();
            populateDatumInfo(e, def);
            addDefToPduDef(def, KDIS::DATA_TYPE::ENUMS::IsPartOf_PDU_Type, defs);
        }
        else if(source == "MinefieldState")
        {
            BaseDef* def = new BaseDef();
            populateDatumInfo(e, def);
            addDefToPduDef(def, KDIS::DATA_TYPE::ENUMS::MinefieldState_PDU_Type, defs);
        }
        else if(source == "MinefieldQuery")
        {
            BaseDef* def = new BaseDef();
            populateDatumInfo(e, def);
            addDefToPduDef(def, KDIS::DATA_TYPE::ENUMS::MinefieldQuery_PDU_Type, defs);
        }
        else if(source == "MinefieldData")
        {
            BaseDef* def = new BaseDef();
            populateDatumInfo(e, def);
            addDefToPduDef(def, KDIS::DATA_TYPE::ENUMS::MinefieldData_PDU_Type, defs);
        }
        else if(source == "MinefieldResponseNak")
        {
            BaseDef* def = new BaseDef();
            populateDatumInfo(e, def);
            addDefToPduDef(def, KDIS::DATA_TYPE::ENUMS::MinefieldResponseNAK_PDU_Type, defs);
        }
        else if(source == "EnvironmentalProcess")
        {
            BaseDef* def = new BaseDef();
            populateDatumInfo(e, def);
            addDefToPduDef(def, KDIS::DATA_TYPE::ENUMS::EnvironmentalProcess_PDU_Type, defs);
        }
        else if(source == "GriddedData")
        {
            BaseDef* def = new BaseDef();
            populateDatumInfo(e, def);
            addDefToPduDef(def, KDIS::DATA_TYPE::ENUMS::GriddedData_PDU_Type, defs);
        }
        else if(source == "PointObjectState")
        {
            BaseDef* def = new BaseDef();
            populateDatumInfo(e, def);
            addDefToPduDef(def, KDIS::DATA_TYPE::ENUMS::PointObjectState_PDU_Type, defs);
        }
        else if(source == "LinearObjectState")
        {
            BaseDef* def = new BaseDef();
            populateDatumInfo(e, def);
            addDefToPduDef(def, KDIS::DATA_TYPE::ENUMS::LinearObjectState_PDU_Type, defs);
        }
        else if(source == "ArealObjectState")
        {
            BaseDef* def = new BaseDef();
            populateDatumInfo(e, def);
            addDefToPduDef(def, KDIS::DATA_TYPE::ENUMS::ArealObjectState_PDU_Type, defs);
        }
        else if(source == "TSPI")
        {
            BaseDef* def = new BaseDef();
            populateDatumInfo(e, def);
            addDefToPduDef(def, KDIS::DATA_TYPE::ENUMS::TSPI_PDU_Type, defs);
        }
        else if(source == "Appearance")
        {
            BaseDef* def = new BaseDef();
            populateDatumInfo(e, def);
            addDefToPduDef(def, KDIS::DATA_TYPE::ENUMS::Appearance_PDU_Type, defs);
        }
        else if(source == "ArticulatedParts")
        {
            BaseDef* def = new BaseDef();
            populateDatumInfo(e, def);
            addDefToPduDef(def, KDIS::DATA_TYPE::ENUMS::ArticulatedParts_PDU_Type, defs);
        }
        else if(source == "LEFire")
        {
            BaseDef* def = new BaseDef();
            populateDatumInfo(e, def);
            addDefToPduDef(def, KDIS::DATA_TYPE::ENUMS::LEFire_PDU_Type, defs);
        }
        else if(source == "LEDetonation")
        {
            BaseDef* def = new BaseDef();
            populateDatumInfo(e, def);
            addDefToPduDef(def, KDIS::DATA_TYPE::ENUMS::LEDetonation_PDU_Type, defs);
        }
        else if(source == "CreateEntityR")
        {
            BaseDef* def = new BaseDef();
            populateDatumInfo(e, def);
            addDefToPduDef(def, KDIS::DATA_TYPE::ENUMS::CreateEntity_R_PDU_Type, defs);
        }
        else if(source == "RemoveEntityR")
        {
            BaseDef* def = new BaseDef();
            populateDatumInfo(e, def);
            addDefToPduDef(def, KDIS::DATA_TYPE::ENUMS::RemoveEntity_R_PDU_Type, defs);
        }
        else if(source == "StartResumeR")
        {
            BaseDef* def = new BaseDef();
            populateDatumInfo(e, def);
            addDefToPduDef(def, KDIS::DATA_TYPE::ENUMS::Start_Resume_R_PDU_Type, defs);
        }
        else if(source == "StopFreezeR")
        {
            BaseDef* def = new BaseDef();
            populateDatumInfo(e, def);
            addDefToPduDef(def, KDIS::DATA_TYPE::ENUMS::Stop_Freeze_R_PDU_Type, defs);
        }
        else if(source == "AcknowledgeR")
        {
            BaseDef* def = new BaseDef();
            populateDatumInfo(e, def);
            addDefToPduDef(def, KDIS::DATA_TYPE::ENUMS::Acknowledge_R_PDU_Type, defs);
        }
        else if(source == "ActionRequestR")
        {
            BaseDef* def = new BaseDef();
            populateDatumInfo(e, def);
            addDefToPduDef(def, KDIS::DATA_TYPE::ENUMS::ActionRequest_R_PDU_Type, defs);
        }
        else if(source == "ActionResponseR")
        {
            BaseDef* def = new BaseDef();
            populateDatumInfo(e, def);
            addDefToPduDef(def, KDIS::DATA_TYPE::ENUMS::ActionResponse_R_PDU_Type, defs);
        }
        else if(source == "DataQueryR")
        {
            BaseDef* def = new BaseDef();
            populateDatumInfo(e, def);
            addDefToPduDef(def, KDIS::DATA_TYPE::ENUMS::DataQuery_R_PDU_Type, defs);
        }
        else if(source == "SetDataR")
        {
            BaseDef* def = new BaseDef();
            populateDatumInfo(e, def);
            addDefToPduDef(def, KDIS::DATA_TYPE::ENUMS::SetData_R_PDU_Type, defs);
        }
        else if(source == "DataR")
        {
            BaseDef* def = new BaseDef();
            populateDatumInfo(e, def);
            addDefToPduDef(def, KDIS::DATA_TYPE::ENUMS::Data_R_PDU_Type, defs);
        }
        else if(source == "EventReportR")
        {
            BaseDef* def = new BaseDef();
            populateDatumInfo(e, def);
            addDefToPduDef(def, KDIS::DATA_TYPE::ENUMS::EventReport_R_PDU_Type, defs);
        }
        else if(source == "CommentR")
        {
            BaseDef* def = new BaseDef();
            populateDatumInfo(e, def);
            addDefToPduDef(def, KDIS::DATA_TYPE::ENUMS::Comment_R_PDU_Type, defs);
        }
        else if(source == "RecordR")
        {
            BaseDef* def = new BaseDef();
            populateDatumInfo(e, def);
            addDefToPduDef(def, KDIS::DATA_TYPE::ENUMS::Record_R_PDU_Type, defs);
        }
        else if(source == "SetRecordR")
        {
            BaseDef* def = new BaseDef();
            populateDatumInfo(e, def);
            addDefToPduDef(def, KDIS::DATA_TYPE::ENUMS::SetRecord_R_PDU_Type, defs);
        }
        else if(source == "RecordQueryR")
        {
            BaseDef* def = new BaseDef();
            populateDatumInfo(e, def);
            addDefToPduDef(def, KDIS::DATA_TYPE::ENUMS::RecordQuery_R_PDU_Type, defs);
        }
        else if(source == "CollisionElastic")
        {
            BaseDef* def = new BaseDef();
            populateDatumInfo(e, def);
            addDefToPduDef(def, KDIS::DATA_TYPE::ENUMS::Collision_Elastic_PDU_Type, defs);
        }
        else if(source == "EntityStateUpdate")
        {
            BaseDef* def = new BaseDef();
            populateDatumInfo(e, def);
            addDefToPduDef(def, KDIS::DATA_TYPE::ENUMS::EntityStateUpdate_PDU_Type, defs);
        }
        else if(source == "DirectedEnergyFire")
        {
            BaseDef* def = new BaseDef();
            populateDatumInfo(e, def);
            addDefToPduDef(def, KDIS::DATA_TYPE::ENUMS::DirectedEnergyFire_PDU_Type, defs);
        }
        else if(source == "EntityDamageStatus")
        {
            BaseDef* def = new BaseDef();
            populateDatumInfo(e, def);
            addDefToPduDef(def, KDIS::DATA_TYPE::ENUMS::EntityDamageStatus_PDU_Type, defs);
        }
        else if(source == "IO_Action")
        {
            BaseDef* def = new BaseDef();
            populateDatumInfo(e, def);
            addDefToPduDef(def, KDIS::DATA_TYPE::ENUMS::IO_Action_PDU_Type, defs);
        }
        else if(source == "IO_Report")
        {
            BaseDef* def = new BaseDef();
            populateDatumInfo(e, def);
            addDefToPduDef(def, KDIS::DATA_TYPE::ENUMS::IO_Report_PDU_Type, defs);
        }
        else if(source == "Attribute")
        {
            BaseDef* def = new BaseDef();
            populateDatumInfo(e, def);
            addDefToPduDef(def, KDIS::DATA_TYPE::ENUMS::Attribute_PDU_Type, defs);
        }
        else if(source == "AnnounceObject")
        {
            BaseDef* def = new BaseDef();
            populateDatumInfo(e, def);
            addDefToPduDef(def, KDIS::DATA_TYPE::ENUMS::Announce_Object_PDU_Type, defs);
        }
        else if(source == "DeleteObject")
        {
            BaseDef* def = new BaseDef();
            populateDatumInfo(e, def);
            addDefToPduDef(def, KDIS::DATA_TYPE::ENUMS::Delete_Object_PDU_Type, defs);
        }
        else if(source == "DescribeEvent")
        {
            BaseDef* def = new BaseDef();
            populateDatumInfo(e, def);
            addDefToPduDef(def, KDIS::DATA_TYPE::ENUMS::Describe_Event_PDU_Type, defs);
        }
        else if(source == "DescribeObject")
        {
            BaseDef* def = new BaseDef();
            populateDatumInfo(e, def);
            addDefToPduDef(def, KDIS::DATA_TYPE::ENUMS::Describe_Object_PDU_Type, defs);
        }
        else if(source == "RequestEvent")
        {
            BaseDef* def = new BaseDef();
            populateDatumInfo(e, def);
            addDefToPduDef(def, KDIS::DATA_TYPE::ENUMS::Request_Event_PDU_Type, defs);
        }
        else if(source == "RequestObject")
        {
            BaseDef* def = new BaseDef();
            populateDatumInfo(e, def);
            addDefToPduDef(def, KDIS::DATA_TYPE::ENUMS::Request_Object_PDU_Type, defs);
        }
        else if(source == "FixedDatum")
        {
            uint32_t fixedId = id.toUInt();
            // Fixed datums can come in Data or SetData PDUs
            BaseDef* def = new BaseDef();
            populateDatumInfo(e, def);
            def->definition_id.setFixedType(fixedId);
            addDefToPduDef(def, KDIS::DATA_TYPE::ENUMS::Data_PDU_Type, defs);

            def = new BaseDef();
            populateDatumInfo(e, def);
            def->definition_id.setFixedType(fixedId);
            addDefToPduDef(def, KDIS::DATA_TYPE::ENUMS::Set_Data_PDU_Type, defs);
        }
        else if(source == "VariableDatum")
        {
            uint32_t varId = id.toUInt();
            // Variable datums can come in Data or SetData PDUs
            BaseDef* def = new BaseDef();
            populateDatumInfo(e, def);
            def->definition_id.setVariableType(varId);
            addDefToPduDef(def, KDIS::DATA_TYPE::ENUMS::Data_PDU_Type, defs);

            def = new BaseDef();
            populateDatumInfo(e, def);
            def->definition_id.setVariableType(varId);
            addDefToPduDef(def, KDIS::DATA_TYPE::ENUMS::Set_Data_PDU_Type, defs);
        }
        else
        {
            std::cerr << "Unknown source type:" << source.toStdString() << std::endl;
            file_error_detected = true;
        }
        e = e.nextSibling().toElement();
    }
}

void VehicleMetadataLoader::populateDatumInfo(QDomElement singleDatumInfo, DatumDef* def)
{
    // Parse attributes
    QString source, id, varType, offset, size, byteOrder;
    source    = singleDatumInfo.attribute("source", "UNKNOWN");
    id        = singleDatumInfo.attribute("id", "UNKNOWN");
    varType   = singleDatumInfo.attribute("type", "uint32");
    offset    = singleDatumInfo.attribute("offset", "0");
    size      = singleDatumInfo.attribute("size", "4");
    byteOrder = singleDatumInfo.attribute("byte_order", "network");

    def->setType(varType.toStdString());
    def->setOffset(offset.toUInt());
    def->setLength(size.toUInt());
    def->setByteOrder(byteOrder.toStdString());

    // Parse children
    QString name(""), category(""), description(""), unitClass(""),
            unit(""), minVal(""), maxVal(""), enumType("");
    QDomElement child = singleDatumInfo.firstChild().toElement();
    while(!child.isNull())
    {
        QString tag = child.tagName();
        QString tagVal = child.firstChild().toText().data();
        if(tag == "Name")
            name = tagVal;
        else if(tag == "Category")
            category = tagVal;
        else if(tag == "Description")
            description = tagVal;
        else if(tag == "UnitClass")
            unitClass = tagVal;
        else if(tag == "Unit")
            unit = tagVal;
        else if(tag == "MinValue")
            minVal = tagVal;
        else if(tag == "MaxValue")
            maxVal = tagVal;
        else if(tag == "EnumType")
            enumType = tagVal;
        else
        {
            std::cerr << "Uknown DatumInfo node tag:" << tag.toStdString() << std::endl;
            file_error_detected = true;
        }
        child = child.nextSibling().toElement();
    }

    def->setName(name.toStdString());
    def->setCategory(category.toStdString());
    def->setDescription(description.toStdString());
    def->setUnitClass(unitClass.toStdString());
    def->setUnit(unit.toStdString());
    if(minVal != "") def->setMin(minVal.toStdString(), varType.toStdString());
    if(maxVal != "") def->setMax(maxVal.toStdString(), varType.toStdString());
    def->setEnumType(enumType.toStdString());
}

void VehicleMetadataLoader::addDefToPduDef(BaseDef* def, uint8_t pduType,
                                          std::map<uint8_t, PduDef*>* defs)
{
    PduDef* mainDef;
    if(defs->count(pduType) > 0)
    {
        mainDef = (*defs)[pduType];
    }
    else
    {
        mainDef = new PduDef();
        (*defs)[pduType] = mainDef;
    }
    mainDef->add(def);
}



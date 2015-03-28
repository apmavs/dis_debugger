#include "VehicleMetadataLoader.h"

#include <QFile>
#include <iostream>
#include "UnitClassDef.h"

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

void VehicleMetadataLoader::load(PduDefMap & pduDefs, UnitClassDefMap & classDefs)
{
    // Parse contents of opened XML file
    QDomElement root = xml_doc.documentElement();
    for(QDomNode n = root.firstChild(); !n.isNull(); n = n.nextSibling())
    {
        QDomElement e = n.toElement();

        if(e.tagName() == "UnitDefinitions")
        {
            loadUnitClassDefs(e, classDefs);
        }
        else if(e.tagName() == "DatumDefinitions")
        {
            createDatumDefinitions(e, pduDefs);
        }
    }
}

int VehicleMetadataLoader::lookupPduType(const QString & typeStr) {
    static struct {
        const char *name;
        uint8_t value;
    } typeTable[] = {
        {"EntityState",             KDIS::DATA_TYPE::ENUMS::Entity_State_PDU_Type},
        {"Other",                   KDIS::DATA_TYPE::ENUMS::Other_PDU_Type},
        {"Fire",                    KDIS::DATA_TYPE::ENUMS::Fire_PDU_Type},
        {"Detonation",              KDIS::DATA_TYPE::ENUMS::Detonation_PDU_Type},
        {"Collision",               KDIS::DATA_TYPE::ENUMS::Collision_PDU_Type},
        {"ServiceRequest",          KDIS::DATA_TYPE::ENUMS::Service_Request_PDU_Type},
        {"ResupplyOffer",           KDIS::DATA_TYPE::ENUMS::Resupply_Offer_PDU_Type},
        {"ResupplyReceived",        KDIS::DATA_TYPE::ENUMS::Resupply_Received_PDU_Type},
        {"ResupplyCancel",          KDIS::DATA_TYPE::ENUMS::Resupply_Cancel_PDU_Type},
        {"RepairComplete",          KDIS::DATA_TYPE::ENUMS::Repair_Complete_PDU_Type},
        {"RepairResponse",          KDIS::DATA_TYPE::ENUMS::Repair_Response_PDU_Type},
        {"CreateEntity",            KDIS::DATA_TYPE::ENUMS::Create_Entity_PDU_Type},
        {"RemoveEntity",            KDIS::DATA_TYPE::ENUMS::Remove_Entity_PDU_Type},
        {"StartResume",             KDIS::DATA_TYPE::ENUMS::Start_Resume_PDU_Type},
        {"StopFreeze",              KDIS::DATA_TYPE::ENUMS::Stop_Freeze_PDU_Type},
        {"Acknowledge",             KDIS::DATA_TYPE::ENUMS::Acknowledge_PDU_Type},
        {"ActionRequest",           KDIS::DATA_TYPE::ENUMS::Action_Request_PDU_Type},
        {"ActionResponse",          KDIS::DATA_TYPE::ENUMS::Action_Response_PDU_Type},
        {"DataQuery",               KDIS::DATA_TYPE::ENUMS::Data_Query_PDU_Type},
        {"EventReport",             KDIS::DATA_TYPE::ENUMS::Event_Report_PDU_Type},
        {"Message",                 KDIS::DATA_TYPE::ENUMS::Message_PDU_Type},
        {"ElectromagneticEmission", KDIS::DATA_TYPE::ENUMS::Electromagnetic_Emission_PDU_Type},
        {"Designator",              KDIS::DATA_TYPE::ENUMS::Designator_PDU_Type},
        {"Transmitter",             KDIS::DATA_TYPE::ENUMS::Transmitter_PDU_Type},
        {"Signal",                  KDIS::DATA_TYPE::ENUMS::Signal_PDU_Type},
        {"Receiver",                KDIS::DATA_TYPE::ENUMS::Receiver_PDU_Type},
        {"IFF_ATC_NAVAID",          KDIS::DATA_TYPE::ENUMS::IFF_ATC_NAVAIDS_PDU_Type},
        {"UnderwaterAcoustic",      KDIS::DATA_TYPE::ENUMS::UnderwaterAcoustic_PDU_Type},
        {"SupplementalEmission",    KDIS::DATA_TYPE::ENUMS::SupplementalEmission_EntityState_PDU_Type},
        {"IntercomSignal",          KDIS::DATA_TYPE::ENUMS::IntercomSignal_PDU_Type},
        {"IntercomControl",         KDIS::DATA_TYPE::ENUMS::IntercomControl_PDU_Type},
        {"AggregateState",          KDIS::DATA_TYPE::ENUMS::AggregateState_PDU_Type},
        {"IsGroupOf",               KDIS::DATA_TYPE::ENUMS::IsGroupOf_PDU_Type},
        {"TransferControl",         KDIS::DATA_TYPE::ENUMS::TransferControl_PDU_Type},
        {"IsPartOf",                KDIS::DATA_TYPE::ENUMS::IsPartOf_PDU_Type},
        {"MinefieldState",          KDIS::DATA_TYPE::ENUMS::MinefieldState_PDU_Type},
        {"MinefieldQuery",          KDIS::DATA_TYPE::ENUMS::MinefieldQuery_PDU_Type},
        {"MinefieldData",           KDIS::DATA_TYPE::ENUMS::MinefieldData_PDU_Type},
        {"MinefieldResponseNak",    KDIS::DATA_TYPE::ENUMS::MinefieldResponseNAK_PDU_Type},
        {"EnvironmentalProcess",    KDIS::DATA_TYPE::ENUMS::EnvironmentalProcess_PDU_Type},
        {"GriddedData",             KDIS::DATA_TYPE::ENUMS::GriddedData_PDU_Type},
        {"PointObjectState",        KDIS::DATA_TYPE::ENUMS::PointObjectState_PDU_Type},
        {"LinearObjectState",       KDIS::DATA_TYPE::ENUMS::LinearObjectState_PDU_Type},
        {"ArealObjectState",        KDIS::DATA_TYPE::ENUMS::ArealObjectState_PDU_Type},
        {"TSPI",                    KDIS::DATA_TYPE::ENUMS::TSPI_PDU_Type},
        {"Appearance",              KDIS::DATA_TYPE::ENUMS::Appearance_PDU_Type},
        {"ArticulatedParts",        KDIS::DATA_TYPE::ENUMS::ArticulatedParts_PDU_Type},
        {"LEFire",                  KDIS::DATA_TYPE::ENUMS::LEFire_PDU_Type},
        {"LEDetonation",            KDIS::DATA_TYPE::ENUMS::LEDetonation_PDU_Type},
        {"CreateEntityR",           KDIS::DATA_TYPE::ENUMS::CreateEntity_R_PDU_Type},
        {"RemoveEntityR",           KDIS::DATA_TYPE::ENUMS::RemoveEntity_R_PDU_Type},
        {"StartResumeR",            KDIS::DATA_TYPE::ENUMS::Start_Resume_R_PDU_Type},
        {"StopFreezeR",             KDIS::DATA_TYPE::ENUMS::Stop_Freeze_R_PDU_Type},
        {"AcknowledgeR",            KDIS::DATA_TYPE::ENUMS::Acknowledge_R_PDU_Type},
        {"ActionRequestR",          KDIS::DATA_TYPE::ENUMS::ActionRequest_R_PDU_Type},
        {"ActionResponseR",         KDIS::DATA_TYPE::ENUMS::ActionResponse_R_PDU_Type},
        {"DataQueryR",              KDIS::DATA_TYPE::ENUMS::DataQuery_R_PDU_Type},
        {"SetDataR",                KDIS::DATA_TYPE::ENUMS::SetData_R_PDU_Type},
        {"DataR",                   KDIS::DATA_TYPE::ENUMS::Data_R_PDU_Type},
        {"EventReportR",            KDIS::DATA_TYPE::ENUMS::EventReport_R_PDU_Type},
        {"CommentR",                KDIS::DATA_TYPE::ENUMS::Comment_R_PDU_Type},
        {"RecordR",                 KDIS::DATA_TYPE::ENUMS::Record_R_PDU_Type},
        {"SetRecordR",              KDIS::DATA_TYPE::ENUMS::SetRecord_R_PDU_Type},
        {"RecordQueryR",            KDIS::DATA_TYPE::ENUMS::RecordQuery_R_PDU_Type},
        {"CollisionElastic",        KDIS::DATA_TYPE::ENUMS::Collision_Elastic_PDU_Type},
        {"EntityStateUpdate",       KDIS::DATA_TYPE::ENUMS::EntityStateUpdate_PDU_Type},
        {"DirectedEnergyFire",      KDIS::DATA_TYPE::ENUMS::DirectedEnergyFire_PDU_Type},
        {"EntityDamageStatus",      KDIS::DATA_TYPE::ENUMS::EntityDamageStatus_PDU_Type},
        {"IO_Action",               KDIS::DATA_TYPE::ENUMS::IO_Action_PDU_Type},
        {"IO_Report",               KDIS::DATA_TYPE::ENUMS::IO_Report_PDU_Type},
        {"Attribute",               KDIS::DATA_TYPE::ENUMS::Attribute_PDU_Type},
        {"AnnounceObject",          KDIS::DATA_TYPE::ENUMS::Announce_Object_PDU_Type},
        {"DeleteObject",            KDIS::DATA_TYPE::ENUMS::Delete_Object_PDU_Type},
        {"DescribeEvent",           KDIS::DATA_TYPE::ENUMS::Describe_Event_PDU_Type},
        {"DescribeObject",          KDIS::DATA_TYPE::ENUMS::Describe_Object_PDU_Type},
        {"RequestEvent",            KDIS::DATA_TYPE::ENUMS::Request_Event_PDU_Type},
        {"RequestObject",           KDIS::DATA_TYPE::ENUMS::Request_Object_PDU_Type},
    };

    for(size_t i = 0; i < sizeof(typeTable) / sizeof(typeTable[0]); i++)
    {
        if(typeStr == typeTable[i].name) return typeTable[i].value;
    }
    return -1;
}

void VehicleMetadataLoader::createDatumDefinitions(QDomElement e, PduDefMap & defs)
{
    e = e.firstChild().toElement();
    while(!e.isNull())
    {
        QString source = e.attribute("source", "UNKNOWN");
        QString id     = e.attribute("id", "UNKNOWN");
        int typeEnum   = lookupPduType(source);

        BaseDef *def = new BaseDef();
        populateDatumInfo(e, def);

        if(typeEnum >= 0)
        {
            addDefToPduDef(def, typeEnum, defs);
        }
        else if(source == "FixedDatum")
        {
            uint32_t fixedId = id.toUInt();
            // Fixed datums can come in Data or SetData PDUs
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
            def->definition_id.setVariableType(varId);
            addDefToPduDef(def, KDIS::DATA_TYPE::ENUMS::Data_PDU_Type, defs);

            def = new BaseDef();
            populateDatumInfo(e, def);
            def->definition_id.setVariableType(varId);
            addDefToPduDef(def, KDIS::DATA_TYPE::ENUMS::Set_Data_PDU_Type, defs);
        }
        else
        {
            std::cerr << "Unknown source type: " << source.toStdString() << std::endl;
            file_error_detected = true;
        }

        e = e.nextSibling().toElement();
    }
}

void VehicleMetadataLoader::loadUnitClassDefs(QDomNode unitDefsNode, UnitClassDefMap & classDefs)
{
    // Read Class elements
    for(QDomNode classNode = unitDefsNode.firstChild(); !classNode.isNull(); classNode = classNode.nextSibling())
    {
        QDomElement classElem = classNode.toElement();
        std::string className = classElem.attributeNode("name").value().toStdString();
        std::string defaultUnitName = classElem.attributeNode("default").value().toStdString();

        UnitClassDef unitClass = UnitClassDef(className, defaultUnitName);
        
        // Read Unit subelements
        for(QDomNode n = classNode.firstChild(); !n.isNull(); n = n.nextSibling())
        {
            QDomElement unitElem = n.toElement();

            // Load output map if any
            EnumDefMap outputMap;

            QString mapStr = unitElem.attribute("map").simplified();
            mapStr.replace(" ", "");

            QStringList pairs = mapStr.split(",", QString::SkipEmptyParts);
            while(!pairs.empty())
            {
                QStringList pair = pairs.takeLast().split("=");

                if(pair.count() == 2)
                {
                    outputMap.insert(std::make_pair(pair[1].toStdString(), pair[0].toStdString()));
                }
            }

            // Create unit definition
            UnitDef unitDef = UnitDef(
                unitElem.attribute("format").toStdString(),
                unitElem.attribute("factor").toDouble(),
                outputMap);
            unitClass.addUnit(unitElem.attributeNode("name").value().toStdString(), unitDef);
        }

        classDefs.insert(std::make_pair(className, unitClass));
    }
}

void VehicleMetadataLoader::populateDatumInfo(QDomElement singleDatumInfo, DatumDef *def)
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

void VehicleMetadataLoader::addDefToPduDef(BaseDef* def, uint8_t pduType, PduDefMap & defs)
{
    PduDef* mainDef;
    if(defs.count(pduType) > 0)
    {
        mainDef = defs[pduType];
    }
    else
    {
        mainDef = new PduDef();
        defs[pduType] = mainDef;
    }
    mainDef->add(def);
}



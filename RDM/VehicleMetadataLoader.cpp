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
        std::cout << std::endl;
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

    std::cout << "Source:" << source.toStdString() << std::endl;
    std::cout << "ID:" << id.toStdString() << std::endl;
    std::cout << "Type:" << varType.toStdString() << std::endl;
    std::cout << "Offset:" << offset.toStdString() << std::endl;
    std::cout << "Size:" << size.toStdString() << std::endl;
    std::cout << "ByteOrder:" << byteOrder.toStdString() << std::endl;

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

    std::cout << "Name: " << name.toStdString() << std::endl;
    std::cout << "Category: " << category.toStdString() << std::endl;
    std::cout << "Description: " << description.toStdString() << std::endl;
    std::cout << "UnitClass: " << unitClass.toStdString() << std::endl;
    std::cout << "Unit: " << unit.toStdString() << std::endl;
    std::cout << "MinValue: " << minVal.toStdString() << std::endl;
    std::cout << "MaxValue: " << maxVal.toStdString() << std::endl;
    std::cout << "EnumType: " << enumType.toStdString() << std::endl;
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



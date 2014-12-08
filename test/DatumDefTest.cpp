#include "DatumDef.h"
#include "BaseDef.h"
#include "ComplexDef.h"

#include <sstream>
#include "gtest/gtest.h"
#include "KDIS/DataTypes/Enums/EnumEntityManagement.h"
#include "KDIS/PDU/Entity_Info_Interaction/Entity_State_PDU.h"
#include "KDIS/PDU/Simulation_Management/Data_PDU.h"
#include "KDIS/PDU/Simulation_Management/Set_Data_PDU.h"


class DatumDefTest : public testing::Test
{
public:
    BaseDef* base_entity;
    BaseDef* base_fixed;
    BaseDef* base_variable;
    KDIS::PDU::Entity_State_PDU entity_pdu;
    KDIS::PDU::Data_PDU fixed_pdu;
    KDIS::PDU::Set_Data_PDU variable_pdu;
    KDIS::DATA_TYPE::FixedDatum fixed_datum;
    KDIS::DATA_TYPE::VariableDatum variable_datum;

    const KDIS::DATA_TYPE::ENUMS::DatumID FIXED_TYPE, VARIABLE_TYPE;
    const std::string ENTITY_NAME, ENTITY_CATEGORY, ENTITY_DESCRIPTION;
    const std::string ENTITY_STR;
    const std::string FIXED_NAME, FIXED_CATEGORY, FIXED_DESCRIPTION;
    const std::string VARIABLE_NAME, VARIABLE_CATEGORY, VARIABLE_DESCRIPTION;
    const SenderId SENDER_ENTITY, SENDER_FIXED, SENDER_VARIABLE;
    const uint32_t FIXED_VALUE;
    const double   VARIABLE_VALUE;
    const std::string FIXED_UNIT, VARIABLE_UNIT;

    DatumDefTest() :
        FIXED_TYPE(KDIS::DATA_TYPE::ENUMS::XID),
        VARIABLE_TYPE(KDIS::DATA_TYPE::ENUMS::HealthID),
        ENTITY_NAME("Entity Marking"),
        ENTITY_CATEGORY("Entity Category"),
        ENTITY_DESCRIPTION("Entity Description"),
        ENTITY_STR("MyEntity"),
        FIXED_NAME("Fixed Name"),
        FIXED_CATEGORY("Fixed Category"),
        FIXED_DESCRIPTION("Fixed Description"),
        VARIABLE_NAME("Variable Name"),
        VARIABLE_CATEGORY("Variable Category"),
        VARIABLE_DESCRIPTION("Variable Description"),
        SENDER_ENTITY(332, 5688, 7388),
        SENDER_FIXED(14, 99, 455),
        SENDER_VARIABLE(1, 0, 0),
        FIXED_VALUE(586738234),
        VARIABLE_VALUE(-55.6),
        FIXED_UNIT("m"),
        VARIABLE_UNIT("radians/s")
    {
        base_entity   = NULL;
        base_fixed    = NULL;
        base_variable = NULL;
    }

    virtual ~DatumDefTest(){}

protected:

    void SetUpEntityDef()
    {
        DatumDefId idEntity;
        idEntity.setPduType(KDIS::DATA_TYPE::ENUMS::Entity_State_PDU_Type);
        idEntity.setFixedType(0);
        idEntity.setVariableType(0);
        base_entity = new BaseDef();
        base_entity->setDefinitionId(idEntity);

        // Set up for entity marking
        base_entity->setLength(11);
        base_entity->setOffset(129);
        base_entity->setByteOrder("string");
        base_entity->setType("string");
        base_entity->setUnit("");
        base_entity->setUnitClass("");
        base_entity->setName(ENTITY_NAME);
        base_entity->setCategory(ENTITY_CATEGORY);
        base_entity->setDescription(ENTITY_DESCRIPTION);
        base_entity->setMin(NULL);    // TODO
        base_entity->setMax(NULL);    // TODO
        base_entity->setEnumType(""); // TODO
    }

    void SetUpEntityPdu()
    {
        KDIS::DATA_TYPE::EntityIdentifier entityId;
        entityId.SetSiteID(SENDER_ENTITY.site);
        entityId.SetApplicationID(SENDER_ENTITY.app);
        entityId.SetEntityID(SENDER_ENTITY.entity);
        entity_pdu.SetEntityIdentifier(entityId);
        KDIS::DATA_TYPE::EntityMarking marking;
        KDIS::DATA_TYPE::ENUMS::EntityMarkingCharacterSet set =
                KDIS::DATA_TYPE::ENUMS::ASCII;
        marking.SetEntityMarkingCharacterSet(set);
        marking.SetEntityMarkingString(ENTITY_STR.c_str());
        entity_pdu.SetEntityMarking(marking);
    }

    void SetUpFixedDef()
    {
        DatumDefId idFixed;
        idFixed.setPduType(KDIS::DATA_TYPE::ENUMS::Data_PDU_Type);
        idFixed.setFixedType(FIXED_TYPE);
        idFixed.setVariableType(0);
        base_fixed = new BaseDef();
        base_fixed->setDefinitionId(idFixed);

        base_fixed->setLength(4);
        base_fixed->setOffset(0);
        base_fixed->setByteOrder("network");
        base_fixed->setType("uint32");
        base_fixed->setUnit(FIXED_UNIT);
        base_fixed->setUnitClass("");
        base_fixed->setName(FIXED_NAME);
        base_fixed->setCategory(FIXED_CATEGORY);
        base_fixed->setDescription(FIXED_DESCRIPTION);
        base_fixed->setMin(NULL);    // TODO
        base_fixed->setMax(NULL);    // TODO
        base_fixed->setEnumType(""); // TODO
    }

    void SetUpFixedPdu()
    {
        KDIS::DATA_TYPE::EntityIdentifier fixedId;
        fixedId.SetSiteID(SENDER_FIXED.site);
        fixedId.SetApplicationID(SENDER_FIXED.app);
        fixedId.SetEntityID(SENDER_FIXED.entity);
        fixed_pdu.SetOriginatingEntityID(fixedId);

        fixed_datum.SetDatumID(FIXED_TYPE);
        fixed_datum.SetDatumValue(FIXED_VALUE);
        fixed_pdu.AddFixedDatum(&fixed_datum);
    }

    void SetUpVariableDef()
    {
        DatumDefId idVariable;
        idVariable.setPduType(KDIS::DATA_TYPE::ENUMS::Set_Data_PDU_Type);
        idVariable.setFixedType(0);
        idVariable.setVariableType(VARIABLE_TYPE);
        base_variable = new BaseDef();
        base_variable->setDefinitionId(idVariable);

        base_variable->setLength(8);
        base_variable->setOffset(2);
        base_variable->setByteOrder("network");
        base_variable->setType("double");
        base_variable->setUnit(VARIABLE_UNIT);
        base_variable->setUnitClass("");
        base_variable->setName(VARIABLE_NAME);
        base_variable->setCategory(VARIABLE_CATEGORY);
        base_variable->setDescription(VARIABLE_DESCRIPTION);
        base_variable->setMin(NULL);    // TODO
        base_variable->setMax(NULL);    // TODO
        base_variable->setEnumType(""); // TODO
    }

    void SetUpVariablePdu()
    {
        KDIS::DATA_TYPE::EntityIdentifier variableId;
        variableId.SetSiteID(SENDER_VARIABLE.site);
        variableId.SetApplicationID(SENDER_VARIABLE.app);
        variableId.SetEntityID(SENDER_VARIABLE.entity);
        variable_pdu.SetOriginatingEntityID(variableId);

        // Create structure for variable piece to lie in
        #pragma pack(1)
        struct VarTestStruct
        {
            uint16_t unused;
            double   test_value;
        } test;
        #pragma pack()

        uint64_t* valPtr = (uint64_t*)&VARIABLE_VALUE;
        uint64_t  val = __builtin_bswap64(*valPtr);
        valPtr = (uint64_t*)(&(test.test_value));
        *valPtr = val;
        const char* testPtr = (char*)&test;

        variable_datum.SetDatumID(VARIABLE_TYPE);
        variable_datum.SetDatumValue(testPtr, sizeof(test)*8);
        variable_pdu.AddVariableDatum(&variable_datum);
    }

    // Set up a several base definitions
    virtual void SetUp()
    {
        SetUpEntityDef();
        SetUpEntityPdu();
        SetUpFixedDef();
        SetUpFixedPdu();
        SetUpVariableDef();
        SetUpVariablePdu();
    }

    virtual void TearDown()
    {
        if(base_entity != NULL)
            delete base_entity;
        if(base_fixed != NULL)
            delete base_fixed;
        if(base_variable != NULL)
            delete base_variable;
    }

};


TEST_F(DatumDefTest, EntityDefTest)
{
    KDIS::KDataStream pduStream = entity_pdu.Encode();
    uint16_t bufSize = pduStream.GetBufferSize();
    unsigned char* data = (unsigned char *)pduStream.GetBufferPtr();
    std::vector<DatumInfo*> datums;
    base_entity->getDatums(&entity_pdu, data, bufSize, &datums);
    ASSERT_EQ(1, datums.size());

    DatumInfo* datum = datums[0];
    DatumIdentifier sender = datum->getId();
    EXPECT_EQ(SENDER_ENTITY.site,   sender.getSite());
    EXPECT_EQ(SENDER_ENTITY.app,    sender.getApp());
    EXPECT_EQ(SENDER_ENTITY.entity, sender.getEntity());
    EXPECT_EQ(ENTITY_NAME,          datum->getName());
    EXPECT_EQ(ENTITY_CATEGORY,      datum->getCategory());
    EXPECT_EQ(ENTITY_DESCRIPTION,   datum->getDescription());
    EXPECT_EQ(ENTITY_STR,           datum->getValue());

    delete datum;
}

TEST_F(DatumDefTest, FixedDefTest)
{
    KDIS::KDataStream pduStream = fixed_pdu.Encode();
    uint16_t bufSize = pduStream.GetBufferSize();
    unsigned char* data = (unsigned char *)pduStream.GetBufferPtr();
    std::vector<DatumInfo*> datums;
    ComplexDef complex;
    complex.add(base_fixed);
    base_fixed = NULL; // Let complex handle destruction
    complex.getDatums(&fixed_pdu, data, bufSize, &datums);
    ASSERT_EQ(1, datums.size());

    DatumInfo* datum = datums[0];
    DatumIdentifier sender = datum->getId();
    EXPECT_EQ(SENDER_FIXED.site,   sender.getSite());
    EXPECT_EQ(SENDER_FIXED.app,    sender.getApp());
    EXPECT_EQ(SENDER_FIXED.entity, sender.getEntity());
    EXPECT_EQ(FIXED_NAME,          datum->getName());
    EXPECT_EQ(FIXED_CATEGORY,      datum->getCategory());
    EXPECT_EQ(FIXED_DESCRIPTION,   datum->getDescription());

    std::ostringstream ss;
    ss << FIXED_VALUE;
    std::string val = ss.str();
    EXPECT_EQ(val, datum->getValue());
}

TEST_F(DatumDefTest, VariableDefTest)
{
    KDIS::KDataStream pduStream = variable_pdu.Encode();
    uint16_t bufSize = pduStream.GetBufferSize();
    unsigned char* data = (unsigned char *)pduStream.GetBufferPtr();
    std::vector<DatumInfo*> datums;
    ComplexDef complex;
    complex.add(base_variable);
    base_variable = NULL; // Let complex handle destruction
    complex.getDatums(&variable_pdu, data, bufSize, &datums);
    ASSERT_EQ(1, datums.size());

    DatumInfo* datum = datums[0];
    DatumIdentifier sender = datum->getId();
    EXPECT_EQ(SENDER_VARIABLE.site,   sender.getSite());
    EXPECT_EQ(SENDER_VARIABLE.app,    sender.getApp());
    EXPECT_EQ(SENDER_VARIABLE.entity, sender.getEntity());
    EXPECT_EQ(VARIABLE_NAME,          datum->getName());
    EXPECT_EQ(VARIABLE_CATEGORY,      datum->getCategory());
    EXPECT_EQ(VARIABLE_DESCRIPTION,   datum->getDescription());

    std::ostringstream ss;
    ss << VARIABLE_VALUE;
    std::string val = ss.str();
    EXPECT_EQ(val, datum->getValue());
}







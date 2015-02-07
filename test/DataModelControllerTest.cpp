#include "DataModelController.h"
#include "DatumObserver.h"
#include "DatumInfo.h"

#include "gtest/gtest.h"
#include <QFile>

class DatumObserverTestClass : public DatumObserver
{
public:
    int new_datum_count;
    int new_value_count;
    std::string curVal;
    const DatumInfo* last_new_datum;
    DatumObserverTestClass()
    {
        new_datum_count = 0;
        new_value_count = 0;
        curVal = "";
        last_new_datum = NULL;
    }

    virtual ~DatumObserverTestClass(){}
    virtual void notifyNewDatum(const DatumInfo* datum)
    {
        curVal = datum->getValue();
        new_datum_count++;
        last_new_datum = datum;
    }

    virtual void notifyNewValue(const DatumInfo* datum)
    {
        curVal = datum->getValue();
        new_value_count++;
    }

    virtual void notifyEntityRemoved(std::string){}
    virtual void notifyAllDatumsInvalid(){}
};

TEST(DataModelControllerTest, ObserverTest)
{
    DataModelController* controller = DataModelController::getInstance();
    DatumInfo* datum = DatumInfo::createDatum(0.0, "uint64", QByteArray());

    DatumObserverTestClass* obs = new DatumObserverTestClass();

    // Ensure double registers don't cause an issue
    controller->registerObserver(obs);
    controller->registerObserver(obs);
    controller->registerDatumObserver(obs, datum, false);
    controller->registerDatumObserver(obs, datum, true);

    // Ensure double unregister doesn't cause an issue
    controller->unregisterObserver(obs);
    controller->unregisterObserver(obs);
    controller->unregisterObserver(obs);
    controller->unregisterDatumObserver(obs, datum);
    controller->unregisterDatumObserver(obs, datum);
    controller->unregisterDatumObserver(obs, datum);

    delete datum;
}

TEST(DataModelControllerTest, PduTest)
{
    DataModelController* controller = DataModelController::getInstance();
    DatumObserverTestClass* obs = new DatumObserverTestClass();
    controller->registerObserver(obs);

    // Create test XML file
    const std::string FILE_NAME = "PduTest_TemporaryFile.xml";
    QString qFileName = QString(FILE_NAME.c_str());
    QFile f(qFileName);
    bool fileOpened = f.open(QIODevice::WriteOnly);
    ASSERT_TRUE(fileOpened);
    const char line1[] = "<DatumDefinitions>\n";
    const char line2[] =
            "<DatumInfo source=\"EntityState\" type=\"float\" offset=\"36\" "
            "size=\"4\" byte_order=\"network\">\n";
    const char line3[] = "<Name>X</Name>\n";
    const char line4[] = "<Category>Velocity</Category>\n";
    const char line5[] = "</DatumInfo>\n";
    const char line6[] = "</DatumDefinitions>\n";
    f.write(line1);
    f.write(line2);
    f.write(line3);
    f.write(line4);
    f.write(line5);
    f.write(line6);
    f.close();
    controller->loadMetadataXml(FILE_NAME);

    const KDIS::KFLOAT32 X1 = 535.7,
                         X2 = 5.0;
    KDIS::DATA_TYPE::Vector velocity(X1, 0.0, 0.0);
    KDIS::PDU::Entity_State_PDU pdu;
    pdu.SetEntityLinearVelocity(velocity);
    controller->notifyPdu(5.0, &pdu);   // This should generate a new datum
    QString qVal = QString(obs->curVal.c_str());
    KDIS::KFLOAT32 curFloat = qVal.toFloat();
    EXPECT_EQ(X1, curFloat);
    EXPECT_EQ(1, obs->new_datum_count);
    EXPECT_EQ(0, obs->new_value_count);
    ASSERT_TRUE(obs->last_new_datum != NULL);
    controller->registerDatumObserver(obs, obs->last_new_datum);

    velocity.SetX(X2);
    pdu.SetEntityLinearVelocity(velocity);
    // Trigger a datum change but not a new datum
    controller->notifyPdu(5.0, &pdu);
    qVal = QString(obs->curVal.c_str());
    curFloat = qVal.toFloat();
    EXPECT_EQ(X2, curFloat);
    EXPECT_EQ(1, obs->new_datum_count);
    EXPECT_EQ(1, obs->new_value_count);

    f.remove(); // Delete temporary file
    delete obs;
}

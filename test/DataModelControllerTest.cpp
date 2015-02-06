#include "gtest/gtest.h"
#include "DataModelController.h"
#include "DatumObserver.h"
#include "DatumInfo.h"

class DatumObserverTestClass : public DatumObserver
{
public:
    DatumObserverTestClass(){}
    virtual ~DatumObserverTestClass(){}
    virtual void notifyNewDatum(const DatumInfo*){}
    virtual void notifyNewValue(const DatumInfo*){}
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

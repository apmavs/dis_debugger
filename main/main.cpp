#include <QThread>
#include <iostream>
#include <time.h>

#include "DataModelController.h"
#include "DatumObserver.h"

// KDIS includes
#include "NetworkPduSource.h"
#include "KDIS/Network/Connection.h" // A cross platform connection class.

using namespace std;
using namespace KDIS;
using namespace DATA_TYPE;
using namespace ENUMS;
using namespace UTILS;
using namespace NETWORK;

class MainObs : public DatumObserver
{
public:
    MainObs(){}
    virtual ~MainObs(){}
    virtual void notifyNewDatum(DatumInfo* datum)
    {
        std::cout << "Received new datum:" << (char *)(datum->getValue().getValue().data()) << std::endl;
    }

    virtual void notifyNewValue(DatumInfo* datum)
    {
        std::cout << "Received new value:" << datum->getName() << std::endl;
    }

    virtual void notifyEntityRemoved(std::string entity)
    {

    }
};


int main()
{
    MainObs* obs = new MainObs();
    DataModelController *controller = new DataModelController();
    controller->registerObserver(obs);
    controller->loadMetadataXml("C:\\Comp\\school_work\\CSE\\dis_debugger\\dis_definitions.xml");

    while(true)
        QThread::msleep(1000);

    return 0;
}

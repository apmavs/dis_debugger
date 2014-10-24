#include <QThread>
#include <iostream>
#include <time.h>

#include "DataModelController.h"

// KDIS includes
#include "NetworkPduSource.h"
#include "KDIS/Network/Connection.h" // A cross platform connection class.

using namespace std;
using namespace KDIS;
using namespace DATA_TYPE;
using namespace ENUMS;
using namespace UTILS;
using namespace NETWORK;

int main()
{
    volatile DataModelController *controller = new DataModelController();

    while(true)
        QThread::msleep(1000);

    return 0;
}

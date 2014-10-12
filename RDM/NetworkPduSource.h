#ifndef NETWORKTHREAD_H
#define NETWORKTHREAD_H

#include <QThread>
#include <QMutex>
#include <iostream>
#include <time.h>
#include <vector>
#include <stdint.h>

#include "PduSource.h"
#include "PduObserver.h"

// KDIS includes
#include "Network/Connection.h"

class NetworkPduSource : public PduSource
{
protected:
    const static uint32_t MAX_BUFFER_SIZE;

    std::string address;
    uint32_t port;
    KDIS::NETWORK::Connection connection;


public:
    NetworkPduSource(std::string ifaceAddr, uint32_t ifacePort = 6993);
    ~NetworkPduSource();

    bool addMulticastGroup(std::string multicastAddr);
    bool removeMulticastGroup(std::string multicastAddr);
    virtual void run();
};

#endif // NETWORKTHREAD_H

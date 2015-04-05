#ifndef NETWORKPDUSOURCE_H
#define NETWORKPDUSOURCE_H

#include <QThread>
#include <QMutex>
#include <iostream>
#include <vector>
#include <stdint.h>

#include "PduSource.h"
#include "PduObserver.h"

// KDIS includes
#include "KDIS/Network/Connection.h"

class NetworkPduSource : public PduSource
{
protected:
    const static uint32_t MAX_BUFFER_SIZE;

    std::string address;
    uint32_t port;
    KDIS::NETWORK::Connection connection;
    std::vector<LoggedPdu> pdus_to_log;


public:
    NetworkPduSource(std::string ifaceAddr, uint32_t ifacePort = 6993);
    virtual ~NetworkPduSource();

    virtual void run();
    std::string getBroadcastAddress();
    uint32_t    getBroadcastPort();

    bool saveToLog(std::string filePath);
};

#endif // NETWORKTHREAD_H

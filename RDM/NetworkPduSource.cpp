#include <auto_ptr.h>
#include "NetworkPduSource.h"
#include "KDIS/Extras/PDU_Factory.h"
#include "KDIS/PDU/Header.h"

// Max PDUs to store from network
const uint32_t NetworkPduSource::MAX_BUFFER_SIZE = 100;

NetworkPduSource::NetworkPduSource(std::string ifaceAddr, uint32_t ifacePort) :
    connection(ifaceAddr, ifacePort)
{
    start_time = time(NULL); // current time
    address = ifaceAddr;
    port = ifacePort;
    connection.SetBlockingModeEnabled(true);
    moveToThread(this);
}

NetworkPduSource::~NetworkPduSource()
{
    if(isRunning())
    {
        quit(); // Signal thread to terminate
        wait(); // Wait until thread terminates
    }
}

void NetworkPduSource::run()
{
    if(isRunning())
        setPriority(QThread::HighPriority);
    KDIS::KOCTET raw_buffer[MAX_PDU_SIZE];
    try
    {
        while(isRunning())
        {
            KDIS::KUINT32 ui32Recv = connection.Receive(raw_buffer,
                                                        MAX_PDU_SIZE);
            if(ui32Recv)
            {
                double timeSinceStart = difftime(time(NULL), start_time);
                notifyObservers(timeSinceStart, raw_buffer, ui32Recv);
            }
        }
    }
    catch(std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
}





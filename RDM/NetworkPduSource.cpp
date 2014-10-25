#include <auto_ptr.h>
#include "NetworkPduSource.h"
#include "Extras/PDU_Factory.h"
#include "PDU/Header.h"

// Max PDUs to store from network
const uint32_t NetworkPduSource::MAX_BUFFER_SIZE = 100;

NetworkPduSource::NetworkPduSource(std::string ifaceAddr, uint32_t ifacePort) :
    connection(ifaceAddr, ifacePort)
{
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
                notifyObservers(raw_buffer, ui32Recv);
            }
        }
    }
    catch(std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
}





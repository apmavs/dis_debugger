#include <auto_ptr.h>
#include "NetworkPduSource.h"
#include "Extras/PDU_Factory.h"
#include "PDU/Header.h"

// Max PDUs to store from network
const uint32_t NetworkPduSource::MAX_BUFFER_SIZE = 100;

NetworkPduSource::NetworkPduSource(std::string ifaceAddr, uint32_t ifacePort) :
    connection(network_address, ifacePort)
{
    network_address = ifaceAddr;
    port = ifacePort;
    std::cout << "NetworkPduSource constructed" << std::endl;
}

NetworkPduSource::~NetworkPduSource()
{
    if(isRunning())
    {
        quit(); // Signal thread to terminate
        wait(); // Wait until thread terminates
    }
}

bool NetworkPduSource::addMulticastGroup(std::string multicastAddr)
{
    bool retVal = true;

    try
    {
        connection.AddMulticastAddress(multicastAddr);
    }
    catch(std::exception & e)
    {
        std::cerr << "NetworkPduSource: Failed to add multicast " <<
                     multicastAddr << std::endl <<
                     e.what() << std::endl;
        retVal = false;
    }

    return retVal;
}

bool NetworkPduSource::removeMulticastGroup(std::string multicastAddr)
{
    bool retVal = true;

    try
    {
        connection.RemoveMulticastAddress(multicastAddr);
    }

    catch(std::exception & e)
    {
        std::cerr << "NetworkPduSource: Failed to remove multicast " <<
                     multicastAddr << std::endl <<
                     e.what() << std::endl;
        retVal = false;
    }

    return retVal;
}

void NetworkPduSource::run()
{
    std::cout << "NetworkPduSource run called" << std::endl;
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





#include <auto_ptr.h>
#include "NetworkPduSource.h"
#include "KDIS/Extras/PDU_Factory.h"
#include "KDIS/PDU/Header.h"
#include "KDIS/Extras/DIS_Logger_Record.h"
#include <sstream>

// Max PDUs to store from network
const uint32_t NetworkPduSource::MAX_BUFFER_SIZE = 8192;

NetworkPduSource::NetworkPduSource(std::string ifaceAddr, uint32_t ifacePort) :
    connection(ifaceAddr, ifacePort)
{
    address = ifaceAddr;
    port = ifacePort;
    connection.SetBlockingModeEnabled(true);
    pdus_to_log.clear();
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
    LoggedPdu savePdu;

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
                double timeSinceStart = getTimeSinceStart();
                notifyObservers(timeSinceStart, raw_buffer, ui32Recv);

                savePdu.timestamp = timeSinceStart;
                savePdu.pduStream = KDIS::KDataStream(raw_buffer, ui32Recv);
                pdus_to_log.push_back(savePdu);
            }
        }
    }
    catch(std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
}

std::string NetworkPduSource::getBroadcastAddress()
{
    return address;
}

uint32_t    NetworkPduSource::getBroadcastPort()
{
    return port;
}

bool NetworkPduSource::saveToLog(std::string filePath)
{
    bool saved = false;
    if(!pdus_to_log.empty())
    {
        try
        {
            // Subtrace first timestamp from all PDUs so that playback
            // will start immediately
            double offset = pdus_to_log.at(0).timestamp;
            // Turn auto-write to file off
            // Buffer all PDUs into recorder, then save them to the file
            KDIS::UTILS::DIS_Logger_Record recorder(filePath, false);
            std::vector<LoggedPdu>::iterator it;
            for(it = pdus_to_log.begin(); it != pdus_to_log.end(); it++)
            {
                recorder.Record((*it).timestamp - offset, (*it).pduStream);
            }
            recorder.Save();
            saved = true;
        }
        catch(std::exception &e)
        {
            std::cerr << __FILE__ << ": ERROR unable to save log " << filePath
                      << ": " << e.what() << std::endl;
        }
        catch(...)
        {
            std::cerr << __FILE__ << ": ERROR unable to save log " << filePath << std::endl;
        }
    }

    return saved;
}





#include <iostream>
#include <stdint.h>

#include "PduSource.h"

// KDIS includes
#include "KDIS/Extras/PDU_Factory.h"

QTime PduSource::start_time;
bool  PduSource::set_start_time = false;

PduSource::PduSource()
{
    unknown_pdu_count = 0;

    // Set the start time once for the first pdu source created
    if(!set_start_time)
    {
        start_time.start();
        set_start_time = true;
    }
}

PduSource::~PduSource()
{
    if(unknown_pdu_count > 0)
    {
        std::cerr << "Received " << unknown_pdu_count << " unknown pdus" <<
                     std::endl;
    }
}

void PduSource::notifyObservers(double timestamp,
                                KDIS::KOCTET *raw_data,
                                KDIS::KUINT32 size)
{
    KDIS::UTILS::PDU_Factory factory;
    std::auto_ptr<KDIS::PDU::Header> pHeader;
    try
    {
        pHeader = factory.Decode(raw_data, size);
    }
    catch(std::exception &e)
    {
        //std::cerr << "notifyObservers exception getting header:"
        //          << e.what() << std::endl;
    }

    if(pHeader.get())
    {
        KDIS::PDU::Header* pdu = pHeader.get();
        observer_mutex.lock();
        std::vector<PduObserver*>::iterator it;
        try
        {
            for(it = observers.begin(); it != observers.end(); it++)
                (*it)->notifyPdu(timestamp, pdu);
        }
        catch(std::exception &e)
        {
            std::cerr << "notifyObservers exception:" << e.what()
                      << std::endl;
        }

        observer_mutex.unlock();
        pHeader.release();
    }
    else
    {
        unknown_pdu_count++;
    }
}

void PduSource::registerPduObserver(PduObserver *obs)
{
    observer_mutex.lock();
    try
    {
        observers.push_back(obs);
    }
    catch(std::exception &e)
    {
        std::cerr << "registerPduObserver exception:" << e.what() << std::endl;
    }

    observer_mutex.unlock();
}

void PduSource::removePduObserver(PduObserver *obs)
{
    observer_mutex.lock();
    observers.erase(std::remove(observers.begin(), observers.end(), obs),
                    observers.end());
    observer_mutex.unlock();
}

double PduSource::getTimeSinceStart()
{
   return start_time.elapsed() * 0.001;
}

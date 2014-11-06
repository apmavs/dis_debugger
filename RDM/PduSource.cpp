#include <iostream>
#include <stdint.h>

#include "PduSource.h"

// KDIS includes
#include "KDIS/Extras/PDU_Factory.h"

PduSource::PduSource()
{
    unknown_pdu_count = 0;
}

PduSource::~PduSource()
{
    if(unknown_pdu_count > 0)
    {
        std::cerr << "Received " << unknown_pdu_count << " unknown pdus" <<
                     std::endl;
    }
}

void PduSource::notifyObservers(KDIS::KOCTET *raw_data,
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
        KDIS::DATA_TYPE::ENUMS::PDUType type = pHeader->
                                               GetPDUType();
        KDIS::KDataStream s(raw_data, size);
        std::vector<PduObserver*>::iterator it;
        observer_mutex.lock();
        try
        {
            if(type == KDIS::DATA_TYPE::ENUMS::Data_PDU_Type)
            {
                KDIS::PDU::Data_PDU dataPdu;
                dataPdu.Decode(s);
                for(it = observers.begin(); it != observers.end(); it++)
                    (*it)->notifyPdu(dataPdu);
            }
            else if(type == KDIS::DATA_TYPE::ENUMS::Set_Data_PDU_Type)
            {
                KDIS::PDU::Set_Data_PDU setDataPdu;
                setDataPdu.Decode(s);
                for(it = observers.begin(); it != observers.end(); it++)
                    (*it)->notifyPdu(setDataPdu);
            }
            else if(type == KDIS::DATA_TYPE::ENUMS::Entity_State_PDU_Type)
            {
                KDIS::PDU::Entity_State_PDU entityStatePdu;
                entityStatePdu.Decode(s);
                for(it = observers.begin(); it != observers.end(); it++)
                    (*it)->notifyPdu(entityStatePdu);
            }
            else
            {
                unknown_pdu_count++;
            }
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

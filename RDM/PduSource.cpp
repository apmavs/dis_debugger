#include <iostream>
#include <stdint.h>

#include "PduSource.h"

// KDIS includes
#include "KDIS/Extras/PDU_Factory.h"

PduSource::PduSource()
{
}

void PduSource::notifyObservers(KDIS::KOCTET *raw_data,
                                       KDIS::KUINT32 size)
{
    KDIS::UTILS::PDU_Factory factory;
    bool unknownPdu = false;
    std::auto_ptr<KDIS::PDU::Header> pHeader;
    try
    {
        pHeader = factory.Decode(raw_data, size);
    }
    catch(std::exception &e)
    {
        std::cerr << "notifyObservers exception getting header:"
                  << e.what() << std::endl;
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
                unknownPdu = true;
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
        unknownPdu = true;
    }

    if(unknownPdu)
    {
        std::cerr << "Received an unknown PDU:";
        KDIS::KDataStream stream(raw_data, size);
        std::cerr << stream.GetAsString() << std::endl;
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

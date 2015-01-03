#include <iostream>

#include "DataModelController.h"

DataModelController* DataModelController::instance = NULL;

DataModelController::DataModelController()
{
    pdu_source = new NetworkPduSource("127.0.0.1");
    pdu_source->registerPduObserver(this);
    pdu_source->start();

    deconstructor = new PduDeconstructor();
}

DataModelController::~DataModelController()
{
    pdu_source->terminate();
    delete pdu_source;
    delete deconstructor;

    std::vector<DatumInfo*>::iterator it;
    for(it = datums.begin(); it != datums.end(); it++)
        delete *it;

    std::map<DatumObserver*, std::vector<DatumIdentifier>*>::iterator itMap;
    for(itMap = rx_observers.begin(); itMap != rx_observers.end(); itMap++)
        delete itMap->second;

    for(itMap = change_observers.begin(); itMap != change_observers.end(); itMap++)
        delete itMap->second;
}

DataModelController* DataModelController::getInstance()
{
    if(instance == NULL)
    {
        instance = new DataModelController();
    }

    return instance;
}

void DataModelController::processNewDatum(DatumInfo* datum)
{
    std::vector<DatumObserver*> observers;

    // Generate a local list of who is registered to receive new datums while
    //  the mutex is locked. Then after we have the list and have unlocked the
    //  mutex, notify everybody. It's possible that on the notification they
    //  could register for this datum which requires a lock
    mutex.lock();
    datums.push_back(datum);
    std::vector<DatumObserver*>::iterator it;
    for(it = new_datum_observers.begin(); it != new_datum_observers.end(); it++)
    {
        observers.push_back(*it);
    }
    mutex.unlock();

    for(it = observers.begin(); it != observers.end(); it++)
        (*it)->notifyNewDatum(datum);
}

void DataModelController::processDatumChange(DatumInfo* datum, bool newValue)
{
    std::vector<DatumObserver*> observers;

    // Generate a local list of who is registered to receive this datum while
    //  the mutex is locked. Then after we have the list and have unlocked the
    //  mutex, notify everybody. It's possible that on the notification they
    //  could unregister which requires a lock
    mutex.lock();
    std::map<DatumObserver*, std::vector<DatumIdentifier>*>::iterator it;
    // Rx observers always receive notice when a new datum arrives
    for(it = rx_observers.begin(); it != rx_observers.end(); it++)
    {
        std::vector<DatumIdentifier>* datumIds = it->second;
        if(std::find(datumIds->begin(), datumIds->end(), datum->getId())
                != datumIds->end())
        {
            // Observer is registered for this datum type
            observers.push_back(it->first);
        }
    }

    // Change observers only receive notice if the value actually changed
    if(newValue)
    {
        for(it = change_observers.begin(); it != change_observers.end(); it++)
        {
            std::vector<DatumIdentifier>* datumIds = it->second;
            if(std::find(datumIds->begin(), datumIds->end(), datum->getId())
                    != datumIds->end())
            {
                // Observer is registered for this datum type
                observers.push_back(it->first);
            }
        }
    }
    mutex.unlock();

    // Notify everybody from list
    std::vector<DatumObserver*>::iterator obsIt;
    for(obsIt = observers.begin(); obsIt != observers.end(); obsIt++)
        (*obsIt)->notifyNewValue(datum);
}

void DataModelController::processEntityRemoval(KDIS::PDU::Header* pdu)
{
    SenderId sender = DatumDef::getSender(pdu);
    std::string entityName = PduDeconstructor::getEntityIdentifier(sender.site,
                                                                   sender.app,
                                                                   sender.entity);
    std::vector<DatumObserver*> observers;
    mutex.lock();
    std::vector<DatumObserver*>::iterator it;
    for(it = new_datum_observers.begin(); it != new_datum_observers.end(); it++)
    {
        observers.push_back(*it);
    }
    mutex.unlock();

    for(it = observers.begin(); it != observers.end(); it++)
    {
        (*it)->notifyEntityRemoved(entityName);
    }
}

void DataModelController::notifyPdu(double timestamp, KDIS::PDU::Header* pdu)
{
    if(pdu->GetPDUType() == KDIS::DATA_TYPE::ENUMS::Detonation_PDU_Type)
    {
        processEntityRemoval(pdu);
    }
    else
    {
        std::vector<DatumInfo*> newDatums = deconstructor->deconstruct(timestamp, pdu);
        std::vector<DatumInfo*>::iterator newIt;
        for(newIt = newDatums.begin(); newIt != newDatums.end(); newIt++)
        {
            DatumInfo* newDatum = *newIt;
            // Check if we've seen this datum before
            bool seenBefore = false;
            std::vector<DatumInfo*>::iterator existingIt;

            mutex.lock();
            bool newValIsChanged = false;
            for(existingIt = datums.begin(); existingIt != datums.end(); existingIt++)
            {
                DatumInfo* existingDatum = *existingIt;
                if(existingDatum->hasSameId(newDatum))
                {
                    seenBefore = true;
                    // Add updated value to stored datum
                    newValIsChanged = existingDatum->addValue(
                                            newDatum->getLastTimestamp(),
                                            newDatum->getLastRawValue());
                    break;
                }
            }
            mutex.unlock();

            if(seenBefore)
                processDatumChange(newDatum, newValIsChanged);
            else // notify observers of new datum type
                processNewDatum(newDatum);
        }
     }
}

// Remove all datums to start with clean, empty history
void DataModelController::removeAllDatums()
{
    // Notify all observers that we are destroying datums
    std::vector<DatumObserver*>::iterator itObs;
    for(itObs = new_datum_observers.begin();
        itObs != new_datum_observers.end();
        itObs++)
    {
        (*itObs)->notifyAllDatumsInvalid();
    }


    std::map<DatumObserver*, std::vector<DatumIdentifier>*>::iterator itChange;
    for(itChange = rx_observers.begin();
        itChange != rx_observers.end();
        itChange++)
    {
        itChange->first->notifyAllDatumsInvalid();
        delete itChange->second;
    }
    rx_observers.clear();
    for(itChange = change_observers.begin();
        itChange != change_observers.end();
        itChange++)
    {
        itChange->first->notifyAllDatumsInvalid();
        delete itChange->second;
    }
    change_observers.clear();

    // Destroy all datums and clear vector
    std::vector<DatumInfo*>::iterator it;
    for(it = datums.begin(); it != datums.end(); it++)
        delete *it;
    datums.clear();
}

bool DataModelController::loadMetadataXml(std::string filename)
{
    mutex.lock();
    bool retVal = deconstructor->loadXml(filename);
    mutex.unlock();
    return retVal;
}

void DataModelController::registerObserver(DatumObserver* obs)
{
    mutex.lock();
    new_datum_observers.push_back(obs);
    mutex.unlock();
}

void DataModelController::registerDatumObserver(DatumObserver* obs,
                                                const DatumInfo* datum,
                                                bool notifyAlways)
{
    mutex.lock();

    std::vector<DatumIdentifier>* datumIds;

    if(notifyAlways)
    {
        if(rx_observers.count(obs))
        {   // Observer is already registered for at least 1 datum
            datumIds = rx_observers[obs];
        }
        else
        {
            datumIds = new std::vector<DatumIdentifier>();
            rx_observers[obs] = datumIds;
        }
    }
    else
    {
        if(change_observers.count(obs))
        {   // Observer is already registered for at least 1 datum
            datumIds = change_observers[obs];
        }
        else
        {
            datumIds = new std::vector<DatumIdentifier>();
            change_observers[obs] = datumIds;
        }
    }

    // Add new datum id to registry
    datumIds->push_back(datum->getId());

    mutex.unlock();
}

void DataModelController::unregisterObserver(DatumObserver* obs)
{
    mutex.lock();
    new_datum_observers.erase(std::remove(new_datum_observers.begin(),
                                          new_datum_observers.end(), obs),
                              new_datum_observers.end());
    mutex.unlock();
}

void DataModelController::unregisterDatumObserver(DatumObserver* obs, const DatumInfo* datum)
{
    mutex.lock();
    if(rx_observers.count(obs))
    {
        DatumIdentifier id = datum->getId();
        std::vector<DatumIdentifier>* datumIds = rx_observers[obs];
        datumIds->erase(std::remove(datumIds->begin(),
                                    datumIds->end(), id),
                               datumIds->end());

        // If no datums are registered any more, remove observer from map
        if(datumIds->empty())
        {
            rx_observers.erase(obs);
            delete datumIds;
        }
    }
    if(change_observers.count(obs))
    {
        DatumIdentifier id = datum->getId();
        std::vector<DatumIdentifier>* datumIds = change_observers[obs];
        datumIds->erase(std::remove(datumIds->begin(),
                                    datumIds->end(), id),
                               datumIds->end());

        // If no datums are registered any more, remove observer from map
        if(datumIds->empty())
        {
            change_observers.erase(obs);
            delete datumIds;
        }
    }
    mutex.unlock();
}

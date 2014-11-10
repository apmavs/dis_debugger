#include <iostream>

#include "DataModelController.h"

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

void DataModelController::processDatumChange(DatumInfo* datum)
{
    std::vector<DatumObserver*> observers;

    // Generate a local list of who is registered to receive this datum while
    //  the mutex is locked. Then after we have the list and have unlocked the
    //  mutex, notify everybody. It's possible that on the notification they
    //  could unregister which requires a lock
    mutex.lock();
    std::map<DatumObserver*, std::vector<DatumIdentifier>*>::iterator it;
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
    mutex.unlock();

    // Notify everybody from list
    std::vector<DatumObserver*>::iterator obsIt;
    for(obsIt = observers.begin(); obsIt != observers.end(); obsIt++)
        (*obsIt)->notifyNewValue(datum);
}

void DataModelController::processEntityRemoval(KDIS::PDU::Header* pdu)
{
    uint16_t site   = 0;
    uint16_t app    = 0;
    uint16_t entity = 0;

    KDIS::KUINT16 headerSize = KDIS::PDU::Header::HEADER6_PDU_SIZE;
    KDIS::KUINT16 entityIdSize = KDIS::DATA_TYPE::EntityIdentifier::ENTITY_IDENTIFER_SIZE;
    KDIS::KUINT16 minSize = headerSize + entityIdSize;
    if(pdu->GetPDULength() >= minSize)
    {
        const KDIS::KOCTET *rawData = pdu->Encode().GetBufferPtr();
        rawData += headerSize; // Move past header
        site = *((uint16_t *)rawData);
        rawData += 2;
        app = *((uint16_t *)rawData);
        rawData += 2;
        entity = *((uint16_t *)rawData);
    }
    std::string entityName = QString("(%1:%2:%3)").arg(site)
                                                  .arg(app)
                                                  .arg(entity)
                                                  .toStdString();
    std::cout << "Removing: " << entityName << std::endl;

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

void DataModelController::notifyPdu(KDIS::PDU::Header* pdu)
{
    if(pdu->GetPDUType() == KDIS::DATA_TYPE::ENUMS::Detonation_PDU_Type)
    {
        processEntityRemoval(pdu);
    }
    else
    {
        std::vector<DatumInfo*> newDatums = deconstructor->deconstruct(pdu);
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
            {
                // Only send notifications if updated datum is changed
                if(newValIsChanged)
                    processDatumChange(newDatum);
            }
            else // notify observers of new datum type
            {
                processNewDatum(newDatum);
            }
        }
     }
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

void DataModelController::registerDatumObserver(DatumObserver* obs, DatumInfo* datum)
{
    mutex.lock();

    std::vector<DatumIdentifier>* datumIds;

    if(change_observers.count(obs))
    {   // Observer is already registered for at least 1 datum
        datumIds = change_observers[obs];
    }
    else
    {
        datumIds = new std::vector<DatumIdentifier>();
        change_observers[obs] = datumIds;
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

void DataModelController::unregisterDatumObserver(DatumObserver* obs, DatumInfo* datum)
{
    mutex.lock();
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

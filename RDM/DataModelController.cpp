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
    existing_datum_types.push_back(datum->getId());
    std::vector<DatumObserver*>::iterator it;
    for(it = new_datum_observers.begin(); it != new_datum_observers.end(); it++)
    {
        (*it)->notifyNewDatum(datum);
    }
}

void DataModelController::processDatumChange(DatumInfo* datum)
{
    // Send new datum to anybody registered for it
    std::map<DatumObserver*, std::vector<DatumIdentifier>*>::iterator it;
    for(it = change_observers.begin(); it != change_observers.end(); it++)
    {
        std::vector<DatumIdentifier>* datumIds = it->second;
        if(std::find(datumIds->begin(), datumIds->end(), datum->getId())
                != datumIds->end())
        {
            // Observer is registered for this datum type
            DatumObserver* obs = it->first;
            obs->notifyNewValue(datum);
        }
    }
}

void DataModelController::notifyPdu(KDIS::PDU::Header pdu)
{
    mutex.lock();
    std::vector<DatumInfo> datums = deconstructor->deconstruct(&pdu);
    std::vector<DatumInfo>::iterator it;
    for(it = datums.begin(); it != datums.end(); it++)
    {
        DatumInfo datum = *it;
        // Check if we've seen this datum before
        if(std::find(existing_datum_types.begin(), existing_datum_types.end(),
                     datum.getId()) != existing_datum_types.end())
        {
            processDatumChange(&datum);
        }
        else // notify observers of new datum type
        {
            processNewDatum(&datum);
        }
    }
    mutex.unlock();
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

#include <iostream>
#include <sstream>

#include "DataModelController.h"

DataModelController* DataModelController::instance = NULL;

DataModelController::DataModelController()
{
    datums.clear();
    new_datum_observers.clear();
    rx_observers.clear();
    change_observers.clear();
    config = Configuration::getInstance();
    std::string ip = config->getValue(CONFIG::BROADCAST_IP);
    uint32_t port = atoi(config->getValue(CONFIG::BROADCAST_PORT).c_str());
    pdu_source = NULL;
    createNetworkConnection(ip, port);

    deconstructor = new PduDeconstructor();
    std::string xml = config->getValue(CONFIG::METADATA_XML);
    loadMetadataXml(xml);
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

void DataModelController::createNetworkConnection(std::string ip, uint32_t port)
{
    // Delete source if it already exists
    if(pdu_source != NULL)
    {
        pdu_source->terminate();
        delete pdu_source;
    }

    pdu_source = new NetworkPduSource(ip, port);
    pdu_source->registerPduObserver(this);
    pdu_source->start();
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
                if(existingDatum->equivalentTo(newDatum))
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

bool DataModelController::loadMetadataXml(std::string filename, bool save)
{
    mutex.lock();
    bool retVal = deconstructor->loadXml(filename);
    if(retVal) config->setValue(CONFIG::METADATA_XML, filename, save);
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

std::string DataModelController::getBroadcastIp()
{
    std::string retVal;
    NetworkPduSource* netSource = dynamic_cast<NetworkPduSource*>(pdu_source);
    if(netSource != NULL)
        retVal = netSource->getBroadcastAddress();
    else
        retVal = config->getValue(CONFIG::BROADCAST_IP);

    return config->getValue(CONFIG::BROADCAST_IP);
}

uint32_t DataModelController::getBroadcastPort()
{
    uint32_t retVal;
    NetworkPduSource* netSource = dynamic_cast<NetworkPduSource*>(pdu_source);
    if(netSource != NULL)
        retVal = netSource->getBroadcastPort();
    else
        retVal = atoi(config->getValue(CONFIG::BROADCAST_PORT).c_str());

    return retVal;
}

bool DataModelController::changeBroadcastIp(std::string newIp, bool save)
{
    bool retVal = false;
    NetworkPduSource* netSource = dynamic_cast<NetworkPduSource*>(pdu_source);
    if(netSource != NULL)
    {
        config->setValue(CONFIG::BROADCAST_IP, newIp, save);
        uint32_t port = netSource->getBroadcastPort();
        createNetworkConnection(newIp, port);
        retVal = true;
    }

    return retVal;
}

bool DataModelController::changeBroadcastPort(uint32_t newPort, bool save)
{
    bool retVal = false;
    NetworkPduSource* netSource = dynamic_cast<NetworkPduSource*>(pdu_source);
    if(netSource != NULL)
    {
        std::ostringstream s;
        s << newPort;
        config->setValue(CONFIG::BROADCAST_PORT, s.str(), save);
        std::string ip = netSource->getBroadcastAddress();
        createNetworkConnection(ip, newPort);
        retVal = true;
    }

    return retVal;
}

DatumInfo* DataModelController::getDatumInfoPtr(std::string datumRep)
{
    DatumInfo* datum = DatumInfo::createFromStringRepresentation(datumRep);
    if(datum != NULL)
    {
        // Check if we are already watching an equivalent datum
        // If so, return a pointer to that datum
        // If not, add it to datums and notify all observers of new datum
        bool foundExisting = false;
        std::vector<DatumInfo*>::iterator it;
        for(it = datums.begin(); it != datums.end(); it++)
        {
            if(datum->equivalentTo(*it))
            {
                delete datum;
                datum = *it;
                foundExisting = true;
                break;
            }
        }

        if(!foundExisting)
        {
            // Delete any history that may have been saved in the datum
            datum->truncateHistory(-1.0);
            processNewDatum(datum);
        }
    }

    return datum;
}

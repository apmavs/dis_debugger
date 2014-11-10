#ifndef DATAMODELCONTROLLER_H
#define DATAMODELCONTROLLER_H

#include "PduObserver.h"
#include "NetworkPduSource.h"
#include "PduDeconstructor.h"
#include "DatumIdentifier.h"
#include "DatumObserver.h"

#include <string>
#include <vector>
#include <map>
#include <QMutex>

class DataModelController : public PduObserver
{
private:
    QMutex mutex;
    PduSource* pdu_source;
    PduDeconstructor* deconstructor;
    std::vector<DatumInfo*> datums;
    std::vector<DatumObserver*> new_datum_observers;
    std::map<DatumObserver*, std::vector<DatumIdentifier>*> change_observers;

    void processNewDatum(DatumInfo* datum);
    void processDatumChange(DatumInfo* datum);
    void processEntityRemoval(KDIS::PDU::Header* pdu);

public:
    DataModelController();
    virtual ~DataModelController();

    virtual void notifyPdu(KDIS::PDU::Header* pdu);

    bool loadMetadataXml(std::string filename);
    void registerObserver(DatumObserver* obs);
    void registerDatumObserver(DatumObserver* obs, DatumInfo* datum);
    void unregisterObserver(DatumObserver* obs);
    void unregisterDatumObserver(DatumObserver* obs, DatumInfo* datum);

};

#endif // DATAMODELCONTROLLER_H

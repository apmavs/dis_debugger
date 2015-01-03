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
    static DataModelController* instance;
    DataModelController();
    virtual ~DataModelController();

    QMutex mutex;
    PduSource* pdu_source;
    PduDeconstructor* deconstructor;
    std::vector<DatumInfo*> datums;
    std::vector<DatumObserver*> new_datum_observers;
    std::map<DatumObserver*, std::vector<DatumIdentifier>*> rx_observers;
    std::map<DatumObserver*, std::vector<DatumIdentifier>*> change_observers;

    void processNewDatum(DatumInfo* datum);
    void processDatumChange(DatumInfo* datum, bool newVal);
    void processEntityRemoval(KDIS::PDU::Header* pdu);

public:
    static DataModelController* getInstance();
    virtual void notifyPdu(double timestamp, KDIS::PDU::Header* pdu);

    void removeAllDatums();
    bool loadMetadataXml(std::string filename);
    void registerObserver(DatumObserver* obs);
    void registerDatumObserver(DatumObserver* obs,
                               const DatumInfo* datum,
                               bool notifyAlways = false);
    void unregisterObserver(DatumObserver* obs);
    void unregisterDatumObserver(DatumObserver* obs, const DatumInfo* datum);

};

#endif // DATAMODELCONTROLLER_H

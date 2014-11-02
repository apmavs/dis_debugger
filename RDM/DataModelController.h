#ifndef DATAMODELCONTROLLER_H
#define DATAMODELCONTROLLER_H

#include "PduObserver.h"
#include "NetworkPduSource.h"
#include "PduDeconstructor.h"

#include <string>

class DataModelController : public PduObserver
{
private:
    PduSource* pdu_source;
    PduDeconstructor* deconstructor;

public:
    DataModelController();
    virtual ~DataModelController();

    bool loadMetadataXml(std::string filename);

    virtual void notifyPdu(KDIS::PDU::Data_PDU pdu);
    virtual void notifyPdu(KDIS::PDU::Set_Data_PDU pdu);
    virtual void notifyPdu(KDIS::PDU::Entity_State_PDU pdu);
};

#endif // DATAMODELCONTROLLER_H

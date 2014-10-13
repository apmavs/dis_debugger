#ifndef PDUOBSERVER_H
#define PDUOBSERVER_H

#include "PDU/Simulation_Management/Data_PDU.h"
#include "PDU/Simulation_Management/Set_Data_PDU.h"
#include "PDU/Entity_Info_Interaction/Entity_State_PDU.h"

class PduObserver
{
public:
    virtual void notifyPdu(KDIS::PDU::Data_PDU pdu)         = 0;
    virtual void notifyPdu(KDIS::PDU::Set_Data_PDU pdu)     = 0;
    virtual void notifyPdu(KDIS::PDU::Entity_State_PDU pdu) = 0;
};

#endif // PDUOBSERVER_H
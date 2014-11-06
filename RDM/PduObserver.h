#ifndef PDUOBSERVER_H
#define PDUOBSERVER_H

#include "KDIS/PDU/Simulation_Management/Data_PDU.h"
#include "KDIS/PDU/Simulation_Management/Set_Data_PDU.h"
#include "KDIS/PDU/Entity_Info_Interaction/Entity_State_PDU.h"

class PduObserver
{
public:
    virtual ~PduObserver(){}
    virtual void notifyPdu(KDIS::PDU::Header pdu) = 0;
};

#endif // PDUOBSERVER_H

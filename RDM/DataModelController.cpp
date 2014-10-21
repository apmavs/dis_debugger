#include <iostream>

#include "DataModelController.h"

DataModelController::DataModelController()
{
    pdu_source = new NetworkPduSource("127.0.0.1");
    pdu_source->registerPduObserver(this);
    pdu_source->start();
}

void DataModelController::notifyPdu(KDIS::PDU::Data_PDU pdu)
{
    std::cout << std::endl;
    std::cout << "Received Data PDU:" << pdu.GetAsString();
    std::cout << std::endl;
}

void DataModelController::notifyPdu(KDIS::PDU::Set_Data_PDU pdu)
{
    std::cout << std::endl;
    std::cout << "Received SetData PDU:" << pdu.GetAsString();
    std::cout << std::endl;
}

void DataModelController::notifyPdu(KDIS::PDU::Entity_State_PDU pdu)
{
    std::cout << std::endl;
    std::cout << "Received EntityState PDU:" << pdu.GetAsString();
    std::cout << std::endl;
}

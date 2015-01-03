#include <QThread>

#include "NetworkPduSource.h"
#include "PduObserver.h"


class NetworkPduSourceTestClass : public NetworkPduSource
{
public:
    // Access to protected members
    std::string getAddress(){ return address;}
    uint32_t getPort(){ return port; }
    KDIS::NETWORK::Connection *getConnection(){ return &connection; }
    std::vector<PduObserver*> *getObservers(){ return &observers; }

    NetworkPduSourceTestClass(std::string addr) :
        NetworkPduSource(addr) {}
    NetworkPduSourceTestClass(std::string addr, uint32_t p) :
        NetworkPduSource(addr, p) {}
    virtual ~NetworkPduSourceTestClass(){}

    void testNotifyObservers(double timestamp, KDIS::KOCTET *raw_data, KDIS::KUINT32 size)
    {
        notifyObservers(timestamp, raw_data, size);
    }
};

class PduObserverTestClass : public PduObserver
{
private:
    double last_time;
    int DataPduCount;
    int SetDataPduCount;
    int EntityStatePduCount;

    KDIS::PDU::Header last_pdu;

public:

    PduObserverTestClass()
    {
        DataPduCount = 0;
        SetDataPduCount = 0;
        EntityStatePduCount = 0;
    }

    virtual ~PduObserverTestClass(){}

    int getDataPduCount(){ return DataPduCount; }
    int getSetDataPduCount(){ return SetDataPduCount; }
    int getEntityStatePduCount(){ return EntityStatePduCount; }

    virtual void notifyPdu(double timestamp, KDIS::PDU::Header* pdu)
    {
        last_time = timestamp;
        uint8_t type = pdu->GetPDUType();
        if(type == KDIS::DATA_TYPE::ENUMS::Entity_State_PDU_Type)
            EntityStatePduCount++;
        else if(type == KDIS::DATA_TYPE::ENUMS::Data_PDU_Type)
            DataPduCount++;
        else if(type == KDIS::DATA_TYPE::ENUMS::Set_Data_PDU_Type)
            SetDataPduCount++;
        last_pdu = *pdu;
    }
};

TEST(NetSourceTest, ObserverDeleteTest)
{
    PduObserverTestClass *o = new PduObserverTestClass();
    delete o;
}

TEST(NetSourceTest, CreateTest)
{
    uint32_t port = 1234;
    std::string iface = "192.168.0.255";
    NetworkPduSourceTestClass net_source(iface, port);
    EXPECT_EQ(port, net_source.getPort()) << "Port did not get set correctly";
    EXPECT_STREQ(iface.c_str(), net_source.getAddress().c_str()) <<
         "Address did not get set correctly";
    net_source.run();
    net_source.terminate();
}

// Test adding and removing observers
TEST(NetSourceTest, ObserverListTest)
{
    const int NUM_OBSERVERS = 500;
    PduObserverTestClass *testObservers[NUM_OBSERVERS];
    NetworkPduSourceTestClass net_source("192.168.0.255");

    // Add observers
    for(int obsNum = 0; obsNum < NUM_OBSERVERS; obsNum++)
    {
        EXPECT_EQ(obsNum, net_source.getObservers()->size());
        try
        {
            testObservers[obsNum] = new PduObserverTestClass();
            net_source.registerPduObserver(testObservers[obsNum]);
        }
        catch(std::exception e){}
    }
    EXPECT_EQ(NUM_OBSERVERS, net_source.getObservers()->size());

    // Remove observers
    for(int obsNum = 0; obsNum < NUM_OBSERVERS; obsNum++)
    {
        EXPECT_EQ((NUM_OBSERVERS - obsNum), net_source.getObservers()->size());
        net_source.removePduObserver(testObservers[obsNum]);
        delete testObservers[obsNum];
    }
    EXPECT_EQ(0, net_source.getObservers()->size());
}


void networkRxTest(std::string iface, int port)
{
    int NUM_PDUS = 50;

    NetworkPduSourceTestClass net_source(iface, port);
    PduObserverTestClass *obs = new PduObserverTestClass();

    net_source.registerPduObserver(obs);
    net_source.start();

    // Send some PDUs out over the network
    KDIS::NETWORK::Connection testTx(iface, port);
    for(int pduNum = 0; pduNum < NUM_PDUS; pduNum++)
    {
        KDIS::PDU::Data_PDU dataPdu;
        KDIS::PDU::Set_Data_PDU setDataPdu;
        KDIS::PDU::Entity_State_PDU entityStatePdu;

        testTx.Send(dataPdu.Encode());
        testTx.Send(setDataPdu.Encode());
        testTx.Send(entityStatePdu.Encode());
        QThread::msleep(5);
    }

    int MAX_WAIT = NUM_PDUS;
    for(int checkIt = 0; checkIt < MAX_WAIT; checkIt++)
    {
        if((obs->getDataPduCount() == NUM_PDUS) &&
           (obs->getSetDataPduCount() == NUM_PDUS) &&
           (obs->getEntityStatePduCount() == NUM_PDUS))
        {
            break; // Received all pdus sent
        }
        QThread::msleep(10);
    }

    EXPECT_EQ(NUM_PDUS, obs->getDataPduCount());
    EXPECT_EQ(NUM_PDUS, obs->getSetDataPduCount());
    EXPECT_EQ(NUM_PDUS, obs->getEntityStatePduCount());

    net_source.terminate();
    delete obs;
}

// Test adding and removing observers
TEST(NetSourceTest, PduRx192Test)
{
    networkRxTest("192.168.0.255", 5544);
}

// Test adding and removing observers
TEST(NetSourceTest, PduRxLoopbackTest)
{
    networkRxTest("127.0.0.1", 6993);
}

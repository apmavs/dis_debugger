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

    void testNotifyObservers(KDIS::KOCTET *raw_data, KDIS::KUINT32 size)
    {
        notifyObservers(raw_data, size);
    }
};

class PduObserverTestClass : public PduObserver
{
public:
    int DataPduCount;
    int SetDataPduCount;
    int EntityStatePduCount;

    PduObserverTestClass()
    {
        DataPduCount = 0;
        SetDataPduCount = 0;
        EntityStatePduCount = 0;
    }

    void notifyPdu(KDIS::PDU::Data_PDU pdu)
    {
        DataPduCount++;
    }

    void notifyPdu(KDIS::PDU::Set_Data_PDU pdu)
    {
        SetDataPduCount++;
    }

    void notifyPdu(KDIS::PDU::Entity_State_PDU pdu)
    {
        EntityStatePduCount++;
    }
};

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
    const int NUM_OBSERVERS = 50000;
    PduObserverTestClass testObservers[NUM_OBSERVERS];
    NetworkPduSourceTestClass net_source("192.168.0.255");

    // Add observers
    for(int obsNum = 0; obsNum < NUM_OBSERVERS; obsNum++)
    {
        EXPECT_EQ(obsNum, net_source.getObservers()->size());
        try{
        net_source.registerPduObserver(&testObservers[obsNum]);
        }
        catch(std::exception e){}
    }
    EXPECT_EQ(NUM_OBSERVERS, net_source.getObservers()->size());

    // Remove observers
    for(int obsNum = 0; obsNum < NUM_OBSERVERS; obsNum++)
    {
        EXPECT_EQ((NUM_OBSERVERS - obsNum), net_source.getObservers()->size());
        net_source.removePduObserver(&testObservers[obsNum]);
    }
    EXPECT_EQ(0, net_source.getObservers()->size());
}

TEST(NetSourceTest, MulticastAddTest)
{
    NetworkPduSourceTestClass net_source("192.168.0.255");

    std::vector<std::string> mcastAddresses;
    mcastAddresses.push_back("224.1.1.1");

    // Test adding multicast addresses
    std::vector<std::string>::iterator it = mcastAddresses.begin();
    for(it; it != mcastAddresses.end(); it++)
    {
        bool added = net_source.addMulticastGroup(*it);
        EXPECT_EQ(true, added);
    }

    // Test removing multicast addresses
    it = mcastAddresses.begin();
    for(it; it != mcastAddresses.end(); it++)
    {
        bool removed = net_source.removeMulticastGroup(*it);
        EXPECT_EQ(true, removed);
    }
}


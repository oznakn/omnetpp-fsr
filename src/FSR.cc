#include <cstdio>
#include <cstring>
#include <omnetpp.h>

#include "FSR_m.h"

#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "inet/routing/base/RoutingProtocolBase.h"
#include "inet/transportlayer/contract/udp/UdpSocket.h"
#include "inet/transportlayer/udp/UdpHeader_m.h"
#include "inet/common/IProtocolRegistrationListener.h"
#include "inet/common/ModuleAccess.h"
#include "inet/common/ProtocolTag_m.h"
#include "inet/common/packet/Packet.h"
#include "inet/common/stlutils.h"
#include "inet/linklayer/common/InterfaceTag_m.h"
#include "inet/networklayer/contract/IL3AddressType.h"
#include "inet/networklayer/common/HopLimitTag_m.h"
#include "inet/networklayer/common/L3AddressResolver.h"
#include "inet/networklayer/common/L3AddressTag_m.h"
#include "inet/networklayer/common/L3Tools.h"
#include "inet/networklayer/ipv4/IcmpHeader.h"
#include "inet/networklayer/ipv4/Ipv4Header_m.h"
#include "inet/networklayer/ipv4/Ipv4Route.h"
#include "inet/transportlayer/common/L4PortTag_m.h"
#include "inet/transportlayer/contract/udp/UdpControlInfo.h"

#define MAX_INT 10000

namespace inet {

class INET_API FSR : public RoutingProtocolBase, public UdpSocket::ICallback, public cListener
{
    private:
        const IL3AddressType *addressType = nullptr;

        double updateInterval = 1.0;
        int scope = 0;

        ModuleRefByPar<IRoutingTable> routingTable;
        UdpSocket socket;

        std::unordered_set<std::string> neighbors;
        std::unordered_map<std::string, int> nodeDist;
        std::unordered_map<std::string, std::string> nodeNextHop;
        std::unordered_map<std::string, std::unordered_set<std::string>> nodeNeighbors;

        cMessage *updateMsgTimer = nullptr;

        void sendPacket(const Ptr<FSRControlPacket>& controlPacket);
        void clearState();
        L3Address getSelfIPAddress() const;

    protected:
        void initialize(int stage) override;
        void handleMessageWhenUp(cMessage *msg) override;
        virtual int numInitStages() const override { return NUM_INIT_STAGES; }

        virtual void socketDataArrived(UdpSocket *socket, Packet *packet) override;
        virtual void socketErrorArrived(UdpSocket *socket, Indication *indication) override;
        virtual void socketClosed(UdpSocket *socket) override;

        virtual void handleStartOperation(LifecycleOperation *operation) override;
        virtual void handleStopOperation(LifecycleOperation *operation) override;
        virtual void handleCrashOperation(LifecycleOperation *operation) override;

    public:
        FSR();
        virtual ~FSR();
};

Define_Module(FSR);

FSR::FSR()
{

}

FSR::~FSR()
{
    this->clearState();
    delete this->updateMsgTimer;
}

void FSR::initialize(int stage)
{
    RoutingProtocolBase::initialize(stage);

    if (stage == INITSTAGE_LOCAL) {
        this->updateInterval = par("updateInterval").doubleValue();
        this->scope = par("scope").intValue();

        this->routingTable.reference(this, "routingTableModule", true);

        this->updateMsgTimer = new cMessage("updateMsgTimer");
    }
}

void FSR::clearState()
{
    cancelEvent(this->helloMsgTimer);
}

void FSR::handleMessageWhenUp(cMessage *msg)
{
    EV_INFO << "FSR received message: " << msg->getName() << endl;

    if (msg->isSelfMessage()) {
        if (msg == updateMsgTimer) {
            // Create and send a hello message
            auto controlPacket = makeShared<FSRUpdatePacket>();
            controlPacket->setPacketType(FSRControlPacketType::UPDATE);
            controlPacket->setChunkLength(B(20));

            controlPacket->setNeighbors(this->nodeNeighbors);

            sendPacket(controlPacket);
            // scheduleAfter(5, helloMsgTimer);
        } else {
            throw cRuntimeError("Unknown self message: %s", msg->getName());
        }
    } else {
        this->socket.processMessage(msg);
    }
}

void FSR::socketDataArrived(UdpSocket *socket, Packet *packet)
{
    EV_INFO << "FSR received packet: " << packet->getName() << endl;
    // L3Address sourceAddr = packet->getTag<L3AddressInd>()->getSrcAddress();
    // const auto& fsrPacket = packet->popAtFront<FSRControlPacket>();
}

void FSR::socketErrorArrived(UdpSocket *socket, Indication *indication)
{
    EV_WARN << "Ignoring UDP error report " << indication->getName() << endl;
    delete indication;
}

void FSR::socketClosed(UdpSocket *socket)
{
}

void FSR::handleStartOperation(LifecycleOperation *operation)
{
    auto selfAddress = getSelfIPAddress();
    this->addressType = selfAddress.getAddressType();

    const char *neighborsStr = par("neighbors");
    cStringTokenizer tokenizer(neighborsStr);
    const char *token;

    while ((token = tokenizer.nextToken()) != nullptr) {
        L3Address result;
        L3AddressResolver().tryResolve(token, result);
        this->neighbors.insert(result.str());
    }

    auto selfAddressStr = selfAddress.str();
    this->nodeDist.insert({selfAddressStr, 0});
    this->nodeNextHop.insert({selfAddressStr, selfAddressStr});
    this->nodeNeighbors.insert({selfAddressStr, this->neighbors});
    for (const auto& neighbor : this->neighbors) {
        this->nodeDist.insert({neighbor, 1});
        this->nodeNextHop.insert({neighbor, neighbor});
        this->nodeNeighbors.insert({neighbor, std::unordered_set<std::string>()});
        this->nodeNeighbors[neighbor].insert(selfAddressStr);
    }

    EV_INFO << "Self Address: " << selfAddressStr << endl;
    EV_INFO << "Neighbors: ";
    for (const auto& neighbor : this->neighbors) {
        EV_INFO << neighbor << " ";
    }
    EV_INFO << endl;

    EV_INFO << "Node Distances: " << endl;
    for (const auto& [node, dist] : this->nodeDist) {
        EV_INFO << "  " << node << ": " << dist << endl;
    }
    EV_INFO << "Node Next Hops: " << endl;
    for (const auto& [node, nextHop] : this->nodeNextHop) {
        EV_INFO << "  " << node << ": " << nextHop << endl;
    }
    EV_INFO << "Node Neighbors: " << endl;
    for (const auto& [node, neighbors] : this->nodeNeighbors) {
        EV_INFO << "  " << node << ": ";
        for (const auto& neighbor : neighbors) {
            EV_INFO << neighbor << " ";
        }
        EV_INFO << endl;
    }
    EV_INFO << "FSR initialized successfully." << endl;

    this->socket.setOutputGate(gate("socketOut"));
    this->socket.bind(L3Address(), 3040);
    this->socket.setBroadcast(true);
    this->socket.setCallback(this);

    scheduleAfter(10.0, helloMsgTimer);
}

void FSR::handleStopOperation(LifecycleOperation *operation)
{
    this->socket.close();
    this->clearState();
}

void FSR::handleCrashOperation(LifecycleOperation *operation)
{
    this->socket.destroy();
    this->clearState();
}

void FSR::sendPacket(const Ptr<FSRControlPacket>& controlPacket) {
    const char *className = controlPacket->getClassName();

    Packet *packet = new Packet(!strncmp("inet::", className, 6) ? className + 6 : className, controlPacket);
    L3Address result;
    L3AddressResolver().tryResolve("145.236.0.2", result);

    for (const auto& neighbor : this->neighbors) {
        L3Address destAddress;
        L3AddressResolver().tryResolve(neighbor.c_str(), destAddress);
        packet->addTag<L3AddressReq>()->setDestAddress(destAddress);
        packet->addTag<L4PortReq>()->setDestPort(3040);
        packet->addTag<HopLimitReq>()->setHopLimit(MAX_INT);

        EV_INFO << "Sending packet: " << packet->getName() << " to " << destAddress << endl;

        this->socket.send(packet);
    }
}

L3Address FSR::getSelfIPAddress() const
{
    return this->routingTable->getRouterIdAsGeneric();
}

}

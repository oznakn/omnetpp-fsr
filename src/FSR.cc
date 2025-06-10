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
        double updateInterval = 1.0;

        ModuleRefByPar<IRoutingTable> routingTable;
        UdpSocket socket;

        int selfNumber = -1;
        std::unordered_map<int, std::unordered_set<int>> neighbors;

        cMessage *updateMsgTimer = nullptr;
        int updateMsgTimerCounter = 0;

        void sendPacket(const Ptr<FSRControlPacket>& controlPacket);
        void clearState();
        L3Address getSelfIPAddress() const;

        void printNeighbors();
        const Ptr<FSRUpdatePacket> createUpdatePacket(int scope);
        void handleUpdatePacket(const Ptr<const FSRUpdatePacket>& updatePacket);

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

        this->routingTable.reference(this, "routingTableModule", true);

        this->updateMsgTimer = new cMessage("updateMsgTimer");
    }
}


void FSR::handleStartOperation(LifecycleOperation *operation)
{
    auto selfAddress = getSelfIPAddress();
    auto selfAddressNumber = selfAddress.toIpv4().getDByte(3);
    this->selfNumber = selfAddressNumber;

    const char *neighborsStr = par("neighbors");
    cStringTokenizer tokenizer(neighborsStr);
    const char *token;

    while ((token = tokenizer.nextToken()) != nullptr) {
        L3Address result;
        L3AddressResolver().tryResolve(token, result);
        auto number = result.toIpv4().getDByte(3);
        this->neighbors[selfAddressNumber].insert(number);
        this->neighbors[number].insert(selfAddressNumber);
    }

    EV_INFO << "FSR: " <<  selfAddressNumber << " initialized with neighbors: " << this->neighbors.size() << endl;
    this->printNeighbors();

    this->socket.setOutputGate(gate("socketOut"));
    this->socket.bind(L3Address(), 3040);
    this->socket.setBroadcast(true);
    this->socket.setCallback(this);

    scheduleAfter(5.0 + this->selfNumber, this->updateMsgTimer);
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

void FSR::clearState()
{
    cancelEvent(this->updateMsgTimer);
}

void FSR::handleMessageWhenUp(cMessage *msg)
{
    EV_INFO << "FSR received message: " << msg->getName() << endl;

    if (!msg->isSelfMessage()) this->socket.processMessage(msg);

    if (msg == this->updateMsgTimer) {
        int scope = 1;
        if (this->updateMsgTimerCounter % 10 == 0) {
            scope = 2;
            this->updateMsgTimerCounter = 0;
        }

        auto packet = createUpdatePacket(scope);

        sendPacket(packet);

        this->updateMsgTimerCounter++;
        scheduleAfter(this->updateInterval, this->updateMsgTimer);
    }
}

void FSR::socketDataArrived(UdpSocket *socket, Packet *packet)
{
    EV_INFO << "FSR received packet: " << packet->getName() << endl;

    if (strcmp(packet->getName(), "FSRUpdatePacket") == 0) {
        auto updatePacket = packet->popAtFront<FSRUpdatePacket>();
        handleUpdatePacket(updatePacket);
    }
}

void FSR::socketErrorArrived(UdpSocket *socket, Indication *indication)
{
    EV_WARN << "Ignoring UDP error report " << indication->getName() << endl;
    delete indication;
}

void FSR::socketClosed(UdpSocket *socket)
{
    this->clearState();
}

void FSR::sendPacket(const Ptr<FSRControlPacket>& controlPacket) {
    const char *className = controlPacket->getClassName();

    for (const auto& neighbor : this->neighbors[this->selfNumber]) {
        std::string address = "145.236.1." + std::to_string(neighbor);
        L3Address destAddress;
        L3AddressResolver().tryResolve(address.c_str(), destAddress);

        Packet *packet = new Packet(!strncmp("inet::", className, 6) ? className + 6 : className, controlPacket);

        packet->addTag<L3AddressReq>()->setDestAddress(destAddress);
        packet->addTag<L4PortReq>()->setDestPort(3040);
        packet->addTag<HopLimitReq>()->setHopLimit(MAX_INT);

        EV_INFO << "Sending packet: " << packet->getName() << " to " << destAddress << endl;

        this->socket.send(packet);
    }
}

const Ptr<FSRUpdatePacket> FSR::createUpdatePacket(int scope)
{
    auto updatePacket = makeShared<FSRUpdatePacket>();
    updatePacket->setPacketType(FSRControlPacketType::UPDATE);
    updatePacket->setChunkLength(B(20));

    std::vector<FSRRoute> routes;
    for (const auto& neighbor : this->neighbors) {
        if (scope == 1 && neighbor.first != this->selfNumber) {
            continue; // Only include self's neighbors for scope 1
        }

        for (const auto& n : neighbor.second) {
            FSRRoute route;
            route.source = neighbor.first;
            route.target = n;
            routes.push_back(route);
        }
    }

    updatePacket->setRoutesArraySize(routes.size());
    for (size_t i = 0; i < routes.size(); ++i) {
        updatePacket->setRoutes(i, routes[i]);
    }

    return updatePacket;
}

void FSR::handleUpdatePacket(const Ptr<const FSRUpdatePacket>& updatePacket)
{
    EV_INFO << "FSR received update packet with " << updatePacket->getRoutesArraySize() << " routes." << endl;

    for (int i = 0; i < updatePacket->getRoutesArraySize(); ++i) {
        const FSRRoute& route = updatePacket->getRoutes(i);

        this->neighbors[route.source].insert(route.target);
        this->neighbors[route.target].insert(route.source);
    }

    this->printNeighbors();
}

void FSR::printNeighbors()
{
    EV_INFO << "FSR Neighbors for node " << this->selfNumber << ":" << endl;
    for (const auto& neighbor : this->neighbors) {
        EV_INFO << "Neighbor: " << neighbor.first << " -> ";
        for (const auto& n : neighbor.second) {
            EV_INFO << n << " ";
        }
        EV_INFO << endl;
    }
}

L3Address FSR::getSelfIPAddress() const
{
    return this->routingTable->getRouterIdAsGeneric();
}

}

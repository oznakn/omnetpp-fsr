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
#include "inet/networklayer/contract/INetfilter.h"
#include "inet/networklayer/contract/IRoutingTable.h"
#include "inet/networklayer/common/HopLimitTag_m.h"
#include "inet/networklayer/common/L3AddressResolver.h"
#include "inet/networklayer/common/L3AddressTag_m.h"
#include "inet/networklayer/common/L3Tools.h"
#include "inet/networklayer/ipv4/IcmpHeader.h"
#include "inet/networklayer/ipv4/Ipv4Header_m.h"
#include "inet/networklayer/ipv4/Ipv4Route.h"
#include "inet/transportlayer/common/L4PortTag_m.h"
#include "inet/transportlayer/contract/udp/UdpControlInfo.h"

#define MAX_INT 1000
#define DEAD_CHECK_INTERVAL 5.0 // seconds
#define DEAD_CHECK_THRESHOLD 5.0 // seconds

namespace inet {

class INET_API FSR : public RoutingProtocolBase, public UdpSocket::ICallback, public cListener
{
    private:
        double updateInterval = 1.0;

        const IL3AddressType *addressType = nullptr;
        NetworkInterface *ifEntry = nullptr;

        ModuleRefByPar<IRoutingTable> routingTable;
        ModuleRefByPar<IInterfaceTable> interfaceTable;
        UdpSocket socket;

        int selfNumber = -1;
        std::unordered_map<int, std::unordered_set<int>> neighbors;
        std::unordered_map<int, simtime_t> lastNodeUpdateTimes;

        cMessage *updateMsgTimer = nullptr;
        int updateMsgTimerCounter = 0;
        cMessage *deadCheckMsgTimer = nullptr;

        void sendPacket(const Ptr<FSRControlPacket>& controlPacket);

        void handleDeadNode(int nodeNumber);
        void updateRoutingTable();
        void printNeighbors();

        const Ptr<FSRUpdatePacket> createUpdatePacket(int scope);
        void handleUpdatePacket(int srcNumber, const Ptr<const FSRUpdatePacket>& updatePacket);

        void clearState();
        L3Address getSelfIPAddress() const;
        L3Address getAddressFromNumber(int number);

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
    delete this->deadCheckMsgTimer;
}

void FSR::initialize(int stage)
{
    RoutingProtocolBase::initialize(stage);

    if (stage == INITSTAGE_LOCAL) {
        this->updateInterval = par("updateInterval").doubleValue();

        this->routingTable.reference(this, "routingTableModule", true);
        this->interfaceTable.reference(this, "interfaceTableModule", true);

        this->updateMsgTimer = new cMessage("updateMsgTimer");
        this->deadCheckMsgTimer = new cMessage("deadCheckMsgTimer");
    }
}


void FSR::handleStartOperation(LifecycleOperation *operation)
{

    this->ifEntry = interfaceTable->findInterfaceByName("wlan0");

    auto selfAddress = getSelfIPAddress();
    auto selfAddressNumber = selfAddress.toIpv4().getDByte(3);
    this->addressType =selfAddress.getAddressType();
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

    this->printNeighbors();

    this->socket.setOutputGate(gate("socketOut"));
    this->socket.bind(L3Address(), 3040);
    this->socket.setBroadcast(true);
    this->socket.setCallback(this);

    scheduleAfter(1.0 + this->selfNumber, this->updateMsgTimer);
    scheduleAfter(DEAD_CHECK_INTERVAL, this->deadCheckMsgTimer);
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
    cancelEvent(this->deadCheckMsgTimer);
}

void FSR::handleMessageWhenUp(cMessage *msg)
{
    // EV_INFO << "FSR received message: " << msg->getName() << endl;

    if (!msg->isSelfMessage()) this->socket.processMessage(msg);

    if (msg == this->deadCheckMsgTimer) {
        std::unordered_set<int> deadNodes;

        // Check for dead neighbors
        simtime_t now = simTime();
        for (auto it = lastNodeUpdateTimes.begin(); it != lastNodeUpdateTimes.end(); it++) {
            if (now - it->second > DEAD_CHECK_THRESHOLD) {
                int deadNodeNumber = it->first;
                EV_INFO << "Node " << deadNodeNumber << " is considered dead." << endl;
                deadNodes.insert(deadNodeNumber);
            }
        }

        if (!deadNodes.empty()) {
            for (const auto& deadNode : deadNodes) {
                lastNodeUpdateTimes.erase(deadNode);
                this->handleDeadNode(deadNode);
            }

            this->routingTable->printRoutingTable();
        }

        scheduleAfter(DEAD_CHECK_INTERVAL, this->deadCheckMsgTimer);
    }

    if (msg == this->updateMsgTimer) {
        int scope = 1;
        if (this->updateMsgTimerCounter % 10 == 0) {
            scope = 2;
            this->updateMsgTimerCounter = 0;
        }

        // scope=2;
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
        // packet source ip
        auto srcAddress = packet->getTag<L3AddressInd>()->getSrcAddress();
        auto srcAddressNumber = srcAddress.toIpv4().getDByte(3);

        auto updatePacket = packet->popAtFront<FSRUpdatePacket>();
        handleUpdatePacket(srcAddressNumber, updatePacket);
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
        L3Address destAddress = this->getAddressFromNumber(neighbor);

        Packet *packet = new Packet(!strncmp("inet::", className, 6) ? className + 6 : className, controlPacket);

        packet->addTag<L3AddressReq>()->setDestAddress(destAddress);
        packet->addTag<L4PortReq>()->setDestPort(3040);
        packet->addTag<HopLimitReq>()->setHopLimit(MAX_INT);

        EV_INFO << "Sending packet " << packet->getName() << " from " << this->selfNumber << " to " << destAddress << endl;

        this->socket.send(packet);
    }
}

const Ptr<FSRUpdatePacket> FSR::createUpdatePacket(int scope)
{
    auto updatePacket = makeShared<FSRUpdatePacket>();
    updatePacket->setPacketType(FSRControlPacketType::UPDATE);
    updatePacket->setChunkLength(B(20));

    std::unordered_set<int> visited;

    std::vector<FSRRoute> routes;
    for (const auto& neighbor : this->neighbors) {
        for (const auto& n : neighbor.second) {
            if (scope == 1 && neighbor.first != this->selfNumber && n != this->selfNumber) {
                continue; // Only include self's neighbors for scope 1
            }

            auto hash = neighbor.first * 10000 + n; // Simple hash to avoid duplicates
            if (visited.find(hash) != visited.end()) {
                continue; // Skip if already visited
            }
            visited.insert(hash);

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

void FSR::handleDeadNode(int nodeNumber)
{
    // Remove the dead node from neighbors
    this->neighbors.erase(nodeNumber);
    for (auto& neighbor : this->neighbors) {
        neighbor.second.erase(nodeNumber);
    }

    // Remove routes to the dead node
    auto route = this->routingTable->findBestMatchingRoute(this->getAddressFromNumber(nodeNumber));
    if (route != nullptr && !route->getNextHopAsGeneric().isUnspecified()) {
        this->routingTable->removeRoute(route);
        EV_INFO << "Removed route to dead node: " << nodeNumber << endl;
    }
}

void FSR::handleUpdatePacket(int srcNumber, const Ptr<const FSRUpdatePacket>& updatePacket)
{
    auto now = simTime();
    lastNodeUpdateTimes[srcNumber] = now;

    for (int i = 0; i < updatePacket->getRoutesArraySize(); ++i) {
        const FSRRoute& route = updatePacket->getRoutes(i);

        auto isSourceIsMyNeighbor = this->neighbors[this->selfNumber].find(route.source) != this->neighbors[this->selfNumber].end();
        auto isTargetIsMyNeighbor = this->neighbors[this->selfNumber].find(route.target) != this->neighbors[this->selfNumber].end();

        if ((isSourceIsMyNeighbor && srcNumber != route.source) || (isTargetIsMyNeighbor && srcNumber != route.target)) {
            continue;
        }

        this->neighbors[route.source].insert(route.target);
        this->neighbors[route.target].insert(route.source);

        this->lastNodeUpdateTimes[route.source] = now;
        this->lastNodeUpdateTimes[route.target] = now;
    }

    this->updateRoutingTable();

    // this->printNeighbors();
    // this->routingTable->printRoutingTable();
}

void FSR::updateRoutingTable()
{
    std::vector<std::vector<int>> destinations;

    for (const auto& neighbor : this->neighbors[this->selfNumber]) {
        std::unordered_set<int> visitedNeighbors;
        visitedNeighbors.insert(this->selfNumber);

        std::vector<std::pair<int, int>> toVisit; // node, distance
        toVisit.push_back({neighbor, 1});

        // Perform a BFS to discover all neighbors
        while (!toVisit.empty()) {
            auto current = toVisit.back();
            toVisit.pop_back();

            if (visitedNeighbors.find(current.first) != visitedNeighbors.end()) {
                continue; // Already visited
            }

            visitedNeighbors.insert(current.first);

            std::vector<int> result; // via, target, distance
            result.push_back(neighbor);
            result.push_back(current.first);
            result.push_back(current.second);
            destinations.push_back(result);

            for (const auto& neighbor : this->neighbors[current.first]) {
                if (visitedNeighbors.find(neighbor) == visitedNeighbors.end()) {
                    toVisit.push_back({neighbor, current.second + 1});
                }
            }
        }
    }

    bool hasChanged = false;

    for (const auto& dest : destinations) {
        EV_INFO << "Adding route via " << dest[0] << " to " << dest[1] << " with distance of " << dest[2] << endl;

        auto midAddress = this->getAddressFromNumber(dest[0]);
        auto destAddress = this->getAddressFromNumber(dest[1]);

        IRoute *route = routingTable->findBestMatchingRoute(destAddress);
        if (route == nullptr) {
            route = this->routingTable->createRoute();
        } else if (route->getNextHopAsGeneric().isUnspecified()) {
            route = this->routingTable->createRoute();
        } else if (route->getMetric() <= dest[2]) {
            continue; // Existing route is better or equal
        }

        hasChanged = true;
        route->setDestination(destAddress);
        route->setNextHop(midAddress);
        route->setMetric(dest[2]);

        route->setInterface(this->ifEntry);
        route->setPrefixLength(this->addressType->getMaxPrefixLength());
        route->setSource(this);

        this->routingTable->addRoute(route);
    }

    if (hasChanged) {
        this->routingTable->printRoutingTable();
    }
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


L3Address FSR::getAddressFromNumber(int number)
{
    std::string addressStr = "145.236.1." + std::to_string(number);
    L3Address address;
    L3AddressResolver().tryResolve(addressStr.c_str(), address);

    return address;
}

L3Address FSR::getSelfIPAddress() const
{
    return this->routingTable->getRouterIdAsGeneric();
}

}

package inet.routing.fsr;

simple FSR
{
    parameters:
        @display("i=block/routing");
        double updateInterval @unit(s) = default(5s);
        double deadCheckInterval @unit(s) = default(30s);
        double deadCheckThreshold @unit(s) = default(60s);
        string neighbors = default("");

        string routingTableModule = default("^.ipv4.routingTable");
        string interfaceTableModule = default("^.interfaceTable");
        string networkProtocolModule = default("^.ipv4.ip");

        @signal[packetSent](type=inet::Packet);
        @statistic[packets](title="packets"; source=packetSent; record=count; unit=pk);
        @statistic[packetLengths](title="packet lengths"; source=packetLength(packetSent); record=sum,histogram,vector; unit=b; interpolationmode=none);

    gates:
        input socketIn;
        output socketOut;
}

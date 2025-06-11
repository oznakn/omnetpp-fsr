package inet.routing.fsr;

simple FSR
{
    parameters:
        @display("i=block/routing");
        double updateInterval @unit(s) = default(5s);
        double deadCheckInterval @unit(s) = default(5s);
        double deadCheckThreshold @unit(s) = default(10s);
        string neighbors = default("");

        string routingTableModule = default("^.ipv4.routingTable");
        string interfaceTableModule = default("^.interfaceTable");
        string networkProtocolModule = default("^.ipv4.ip");

    gates:
        input socketIn;
        output socketOut;
}

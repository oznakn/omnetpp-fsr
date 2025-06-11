package inet.routing.fsr;

simple FSR
{
    parameters:
        @display("i=block/routing");
        double updateInterval @unit(s) = default(3s);
        string neighbors = default("");

        string routingTableModule = default("^.ipv4.routingTable");
        string interfaceTableModule = default("^.interfaceTable");

    gates:
        input socketIn;
        output socketOut;
}

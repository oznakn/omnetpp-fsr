package inet.routing.fsr;

simple FSR
{
    parameters:
        @display("i=block/routing");
        double updateInterval @unit(s) = default(1s);
        int scope = default(3);
        string neighbors = default("");

        string routingTableModule = default("^.ipv4.routingTable");

    gates:
        input socketIn;
        output socketOut;
}

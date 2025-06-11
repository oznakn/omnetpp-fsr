

package inet.routing.fsr;

import inet.node.inet.AdhocHost;
import inet.node.inet.WirelessHost;

module FSRRouter extends AdhocHost
{
    submodules:
        fsr: FSR {
            @display("p=825,226");
        }
    connections:
        fsr.socketOut --> at.in++;
        fsr.socketIn <-- at.out++;
}


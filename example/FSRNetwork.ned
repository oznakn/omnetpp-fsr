package inet.examples.fsr;

import inet.common.scenario.ScenarioManager;
import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;
import inet.networklayer.ipv4.RoutingTableRecorder;
import inet.node.inet.AdhocHost;
import inet.node.inet.WirelessHost;
import inet.physicallayer.wireless.unitdisk.UnitDiskRadioMedium;
import inet.routing.fsr.FSRRouter;

network FSRNetwork
{
    parameters:
        int numHosts = default(2);
        @display("bgb=650,650");
    submodules:
        radioMedium: UnitDiskRadioMedium {
            parameters:
                @display("p=100,200;is=s");
        }
        configurator: Ipv4NetworkConfigurator {
            parameters:
                config = xml("<config><interface hosts='*' address='145.236.1.x' netmask='255.255.0.0'/></config>");
                @display("p=100,100;is=s");
        }
        // routingTableRecorder: RoutingTableRecorder {
        //     parameters:
        //         @display("p=100,300;is=s");
        // }
        // scenarioManager: ScenarioManager {
        //     parameters:
        //         script = default(xml("<scenario/>"));
        //         @display("p=100,400;is=s");
        // }
        host[numHosts]: FSRRouter {
            parameters:
                @display("p=100,300");
        }
    connections allowunconnected:
}

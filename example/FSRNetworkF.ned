package inet.examples.fsr;

import inet.common.scenario.ScenarioManager;
import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;
import inet.networklayer.ipv4.RoutingTableRecorder;
import inet.node.inet.AdhocHost;
import inet.node.inet.WirelessHost;
import inet.physicallayer.wireless.unitdisk.UnitDiskRadioMedium;
import inet.routing.fsr.FSRRouter;

network FSRNetworkF extends FSRNetwork
{
    int numHosts = 20;
    submodules:
        host[numHosts]: FSRRouter {
            parameters:
                @display("p=300,300");
        }
}

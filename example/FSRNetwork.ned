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
        @display("bgb=1000,1000");
    submodules:
        configurator: Ipv4NetworkConfigurator {
            parameters:
                addDefaultRoutes = false;
                addStaticRoutes = false;
                addSubnetRoutes = false;
                config = xml("<config><interface hosts='*' address='145.236.1.x' netmask='255.255.0.0'/></config>");
                @display("p=100,100;is=s");
        }
        radioMedium: UnitDiskRadioMedium {
            parameters:
                @display("p=300,100;is=s");
        }
        routingTableRecorder: RoutingTableRecorder {
            parameters:
                @display("p=500,100;is=s");
        }
        scenarioManager: ScenarioManager {
            parameters:
                script = default(xml("<scenario/>"));
                @display("p=700,100;is=s");
        }
    connections allowunconnected:
}

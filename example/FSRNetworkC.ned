package inet.examples.fsr;

import inet.common.scenario.ScenarioManager;
import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;
import inet.networklayer.ipv4.RoutingTableRecorder;
import inet.node.inet.AdhocHost;
import inet.node.inet.WirelessHost;
import inet.physicallayer.wireless.unitdisk.UnitDiskRadioMedium;
import inet.routing.fsr.FSRRouter;

network FSRNetworkC extends FSRNetwork
{
    submodules:
        hostA: FSRRouter {
            parameters:
                @display("p=300,300");
        }
        hostB: FSRRouter {
            parameters:
                @display("p=300,400");
        }
        hostC: FSRRouter {
            parameters:
                @display("p=400,300");
        }
        hostD: FSRRouter {
            parameters:
                @display("p=400,400");
        }
}

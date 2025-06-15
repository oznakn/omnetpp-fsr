package inet.examples.fsr;

import inet.common.scenario.ScenarioManager;
import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;
import inet.networklayer.ipv4.RoutingTableRecorder;
import inet.node.inet.AdhocHost;
import inet.node.inet.WirelessHost;
import inet.physicallayer.wireless.unitdisk.UnitDiskRadioMedium;
import inet.routing.fsr.FSRRouter;

network FSRNetworkE extends FSRNetwork
{
    submodules:
        hostA: FSRRouter {
            parameters:
                @display("p=300,300");
        }
        hostB: FSRRouter {
            parameters:
                @display("p=200,400");
        }
        hostC: FSRRouter {
            parameters:
                @display("p=400,400");
        }
        hostD: FSRRouter {
            parameters:
                @display("p=300,500");
        }
        hostE: FSRRouter {
            parameters:
                @display("p=600,300");
        }
        hostF: FSRRouter {
            parameters:
                @display("p=500,400");
        }
        hostG: FSRRouter {
            parameters:
                @display("p=700,400");
        }
        hostH: FSRRouter {
            parameters:
                @display("p=600,500");
        }
}

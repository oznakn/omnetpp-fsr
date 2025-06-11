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
        @display("bgb=650,650");

        @figure[rcvdPkText](type=indicatorText; pos=380,300; anchor=w; font=,18; textFormat="packets received: %g"; initialValue=0);
        @statistic[packetReceived](source=hostD.app[0].packetReceived; record=figure(count); targetFigure=rcvdPkText);
    submodules:
        radioMedium: UnitDiskRadioMedium {
            parameters:
                @display("p=100,200;is=s");
        }
        configurator: Ipv4NetworkConfigurator {
            parameters:
                addDefaultRoutes = false;
                addStaticRoutes = false;
                addSubnetRoutes = false;
                config = xml("<config><interface hosts='*' address='145.236.1.x' netmask='255.255.0.0'/></config>");
                @display("p=100,100;is=s");
        }
        routingTableRecorder: RoutingTableRecorder {
            parameters:
                @display("p=100,300;is=s");
        }
        scenarioManager: ScenarioManager {
            parameters:
                script = default(xml("<scenario/>"));
                @display("p=100,400;is=s");
        }
        hostA: FSRRouter {
            parameters:
                @display("p=300,300");
        }
        hostB: FSRRouter {
            parameters:
                @display("p=250,400");
        }
        hostC: FSRRouter {
            parameters:
                @display("p=350,400");
        }
        hostD: FSRRouter {
            parameters:
                @display("p=300,500");
        }
    connections allowunconnected:
}

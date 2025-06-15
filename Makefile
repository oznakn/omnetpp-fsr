update_repo:
	rm -rf repo/example/ repo/src/
	cp -r example/ repo/example/
	cp -r src/ repo/src/

run:
	cd ../inet4.5/examples/fsr && \
		../../../omnetpp-6.1/bin/opp_run -r 0 -m -u Cmdenv -n ..:../../showcases:../../src:../../tests/validation:../../tests/networks:../../tutorials -x "inet.applications.voipstream;inet.common.selfdoc;inet.emulation;inet.examples.emulation;inet.examples.voipstream;inet.linklayer.configurator.gatescheduling.z3;inet.showcases.emulation;inet.showcases.visualizer.osg;inet.transportlayer.tcp_lwip;inet.visualizer.osg" --image-path=../../images -l ../../src/INET omnetpp.ini

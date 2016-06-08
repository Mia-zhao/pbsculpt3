#define DEBUG 0
#define __USE_XBEE__ 1

#include "Arduino.h"
#include "tools.h"
#include "device_types.h"
#include "node.h"

Node node(111111);



void setup()
{
	// Set up node devices
	node.devices[0] = new ActuatedFrondChainDM(1);
	node.devices[1] = new EmptyDeviceModule(2);
	node.devices[2] = new ActuatedFrondChainDM(3);
	node.devices[3] = new EmptyDeviceModule(4);
	node.devices[4] = new EmptyDeviceModule(5);
	node.devices[5] = new ActuatedFrondChainDM(6);

	node.init();
	node._serialNumber = serialCommSetup();

}

void loop()
{
}

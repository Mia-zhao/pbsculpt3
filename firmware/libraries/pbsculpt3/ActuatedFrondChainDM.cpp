/*
 * ActuatedFrondChainDM.cpp
 *
 *  Created on: Jun 7, 2016
 *      Author: dkadish
 */

#include <ActuatedFrondChainDM.h>
#include <highPowerLed.h>
#include <IRProximitySensor.h>

ActuatedFrondChainDM::ActuatedFrondChainDM(char port):
	DeviceModule(port) {

}

ActuatedFrondChainDM::~ActuatedFrondChainDM() {
}


void ActuatedFrondChainDM::init(){
	// Instantiate 1-indexed LEDs
	for( int i = 0; i < _N_LEDS; i++ ){
		HighPowerLED *led = new HighPowerLED(i+1, _port, getPinForAddress(i+1), hasFastPWMForAddress(i+1));
		peripherals.add(led);
	}

	// Instantiate the proximity sensors
	IRProximitySensor *prox = new IRProximitySensor(5, _port, getPinForAddress(5), false);
	peripherals.add(prox);
}

void ActuatedFrondChainDM::loop(){
    for (int i=0; i<peripherals.size(); i++) {
    	peripherals.get(i)->loop();
    }
}

bool ActuatedFrondChainDM::fade(int address, int target, int duration){
	Peripheral* d = getPeripheralAt(address);
    if( d != NULL ){
        d->fade(target, duration);
        return true;
    }
    return false;
}

int  ActuatedFrondChainDM::read(int address, int preprocessType){
	Peripheral* d = getPeripheralAt(address);
    if( d != NULL ){
        return d->read(preprocessType);
    }
    return 0;
}


/* Updates the devList array and returns the new position marker
 */
#include "pindefs.h"
int ActuatedFrondChainDM::getPeripheralList(uint8_t* peripheralList, int position){
	for(int i=1; i <= _N_LEDS; i++){
		peripheralList[position] = _port;
		peripheralList[position+1] = DEVICE_TYPE_HIGH_POWER_LED;
		peripheralList[position+2] = i;
		position += 3;
	}

	return position;
}

int ActuatedFrondChainDM::getValueForAddr(char addr){
	Peripheral* d = getPeripheralAt(addr);
    if( d != NULL ){
        return d->read(0);
    }
    return 0;
}

Peripheral* ActuatedFrondChainDM::getPeripheralAt(char addr){
    for( int i=0; i<peripherals.size(); i++ ){
       if( peripherals.get(i)->address() == addr ){
        return peripherals.get(i);
       }
    }
    return NULL;
}

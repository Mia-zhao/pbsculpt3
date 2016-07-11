/*
 * MinimalCricket.cpp
 *
 *  Created on: Jul 8, 2016
 *      Author: dkadish
 */

#include "MinimalCricket.h"
#include "highPowerLed.h"

#include "tools.h"
#include <stdio.h>

#define CRICKET_1_ADDRESS 1
#define CRICKET_2_ADDRESS 2
#define CRICKET_3_ADDRESS 3
#define CRICKET_4_ADDRESS 4
#define PROX_ADDRESS 5

MinimalCricket::MinimalCricket(char port):
	DeviceModule(port)
{

}

void MinimalCricket::init(){
	DBGLN("Node", "Initializing MinimalCricket.");

	//TODO Change these to crickets (instead of HP LEDs)
    HighPowerLED* led1 = new HighPowerLED(CRICKET_1_ADDRESS, _port,
    		getPinForAddress(CRICKET_1_ADDRESS), hasFastPWMForAddress(CRICKET_1_ADDRESS));
    led1->setBackgroundLevel(1);
    peripherals.add(led1);
    HighPowerLED* led2 = new HighPowerLED(CRICKET_2_ADDRESS, _port,
    		getPinForAddress(CRICKET_2_ADDRESS), hasFastPWMForAddress(CRICKET_2_ADDRESS));
    led2->setBackgroundLevel(1);
    peripherals.add(led2);
    HighPowerLED* led3 = new HighPowerLED(CRICKET_3_ADDRESS, _port,
    		getPinForAddress(CRICKET_3_ADDRESS), hasFastPWMForAddress(CRICKET_3_ADDRESS));
    led3->setBackgroundLevel(1);
    peripherals.add(led3);
    HighPowerLED* led4 = new HighPowerLED(CRICKET_4_ADDRESS, _port,
    		getPinForAddress(CRICKET_4_ADDRESS), hasFastPWMForAddress(CRICKET_4_ADDRESS));
    led4->setBackgroundLevel(1);
    peripherals.add(led4);

    /*AmbientLightSensor* ambientLight = new AmbientLightSensor(PHOTO_ADDRESS, _port,
    		getPinForAddress(PHOTO_ADDRESS), hasFastPWMForAddress(PHOTO_ADDRESS));
    peripherals.add(ambientLight);*/
}

void MinimalCricket::loop(){
	DeviceModule::loop();

    for (int i=0; i<peripherals.size(); i++) {
    	peripherals.get(i)->loop();
    }

}

bool MinimalCricket::fade(int address, int target, int duration){
	Peripheral* d = getPeripheralAt(address);
    if( d != NULL ){
        d->fade(target, duration);
        return true;
    }
    return false;
}

int  MinimalCricket::read(int address, int preprocessType){
	Peripheral* d = getPeripheralAt(address);
    if( d != NULL ){
        return d->read(preprocessType);
    }
    return 0;
}


/* Updates the devList array and returns the new position marker
 */
#include "pindefs.h"
int MinimalCricket::getPeripheralList(uint8_t* peripheralList, int position){
	//TODO Fix this code. Is it necessary anymore?

	/*peripheralList[position] = _port;
	peripheralList[position+1] = 5;
	peripheralList[position+2] = LED_ADDRESS;
	peripheralList[position+3] = _port;
	peripheralList[position+4] = 6;
	peripheralList[position+5] = PHOTO_ADDRESS;
	position += 6;*/

	return position;
}

int MinimalCricket::getValueForAddr(char addr){
	Peripheral* d = getPeripheralAt(addr);
    if( d != NULL ){
        return d->read(0);
    }
    return 0;
}

Peripheral* MinimalCricket::getPeripheralAt(char addr){
    for( int i=0; i<peripherals.size(); i++ ){
       if( peripherals.get(i)->address() == addr ){
        return peripherals.get(i);
       }
    }
    return NULL;
}

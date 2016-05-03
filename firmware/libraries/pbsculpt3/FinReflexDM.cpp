/*
 * FinReflexDM.cpp
 *
 *  Created on: Mar 24, 2016
 *      Author: dkadish
 *
 * Differences from ProprioceptiveLED: define is in .h | 
 */

#include "FinReflexDM.h"
#include "FinSMA.h"

#include "tools.h"
#include <stdio.h>

FinReflexDM::FinReflexDM(char port):
	DeviceModule(port) {

}

void FinReflexDM::init(){
    FinSMA* rightSMA = new FinSMA(__RIGHT_SMA_ADDRESS,
    		getPinForAddress(__RIGHT_SMA_ADDRESS), hasFastPWMForAddress(__RIGHT_SMA_ADDRESS));
    peripherals.add(rightSMA);

    FinSMA* leftSMA = new FinSMA(__LEFT_SMA_ADDRESS,
    		getPinForAddress(__LEFT_SMA_ADDRESS), hasFastPWMForAddress(__LEFT_SMA_ADDRESS));
    peripherals.add(leftSMA);
}

void FinReflexDM::loop(){

    for (int i=0; i<peripherals.size(); i++) {
    	peripherals.get(i)->loop();
    }
}

bool FinReflexDM::fade(int address, int target, int duration){
	Peripheral* d = getPeripheralAt(address);
    if( d != NULL ){
        d->fade(target, duration);
        return true;
    }
    return false;
}

int  FinReflexDM::read(int address, int preprocessType){
	Peripheral* d = getPeripheralAt(address);
    if( d != NULL ){
        return d->read(preprocessType);
    }
    return 0;
}


/* Updates the devList array and returns the new position marker
 */
#include "pindefs.h"
int FinReflexDM::getPeripheralList(uint8_t* peripheralList, int position){
	peripheralList[position] = _port;
	peripheralList[position+1] = DEVICE_TYPE_SMA;
	peripheralList[position+2] = __RIGHT_SMA_ADDRESS;
	peripheralList[position+3] = _port;
	peripheralList[position+4] = DEVICE_TYPE_SMA;
	peripheralList[position+5] = __LEFT_SMA_ADDRESS;
	position += 6;

	return position;
}

int FinReflexDM::getValueForAddr(char addr){
	Peripheral* d = getPeripheralAt(addr);
    if( d != NULL ){
        return d->read(0);
    }
    return 0;
}

Peripheral* FinReflexDM::getPeripheralAt(char addr){
    for( int i=0; i<peripherals.size(); i++ ){
       if( peripherals.get(i)->address() == addr ){
        return peripherals.get(i);
       }
    }
    return NULL;
}

/*
 *
 *
 */

#include "proprioceptiveLed.h"
#include "highPowerLed.h"
#include "AmbientLightSensor.h"

#include "tools.h"
#include <stdio.h>

#define LED_ADDRESS 1
#define PHOTO_ADDRESS 5

ProprioceptiveLed::ProprioceptiveLed(char port):
	DeviceModule(port)
{
    
}

void ProprioceptiveLed::init(){
	DBGLN("Node", "Initializing ProprioceptiveLed.");
    HighPowerLED* led = new HighPowerLED(LED_ADDRESS, _port,
    		getPinForAddress(LED_ADDRESS), hasFastPWMForAddress(LED_ADDRESS));
    peripherals.add(led);

    AmbientLightSensor* ambientLight = new AmbientLightSensor(PHOTO_ADDRESS, _port,
    		getPinForAddress(PHOTO_ADDRESS), hasFastPWMForAddress(PHOTO_ADDRESS));
    peripherals.add(ambientLight);

    DeviceModule::init();
}

bool ProprioceptiveLed::fade(int address, int target, int duration){
	Peripheral* d = getPeripheralAt(address);
    if( d != NULL ){
        d->fade(target, duration);
        return true;
    }
    return false;
}

int  ProprioceptiveLed::read(int address, int preprocessType){
	Peripheral* d = getPeripheralAt(address);
    if( d != NULL ){
        return d->read(preprocessType);
    }
    return 0;
}


/* Updates the devList array and returns the new position marker
 */
#include "pindefs.h"
int ProprioceptiveLed::getPeripheralList(uint8_t* peripheralList, int position){
	peripheralList[position] = _port;
	peripheralList[position+1] = 5;
	peripheralList[position+2] = LED_ADDRESS;
	peripheralList[position+3] = _port;
	peripheralList[position+4] = 6;
	peripheralList[position+5] = PHOTO_ADDRESS;
	position += 6;
   
	return position;
}

int ProprioceptiveLed::getValueForAddr(char addr){
	Peripheral* d = getPeripheralAt(addr);
    if( d != NULL ){
        return d->read(0);
    }
    return 0;
}

Peripheral* ProprioceptiveLed::getPeripheralAt(char addr){
    for( int i=0; i<peripherals.size(); i++ ){
       if( peripherals.get(i)->address() == addr ){
        return peripherals.get(i);
       }
    }
    return NULL;
}

/*
 *
 *
 */

#include "emptyDeviceModule.h"

#include <Arduino.h>

EmptyDeviceModule::EmptyDeviceModule(char port):
	DeviceModule(port)
{
    
}

void EmptyDeviceModule::init(){}

void EmptyDeviceModule::loop(){
    //Serial.println("EmptySubNode Loop");
}

int EmptyDeviceModule::getPeripheralList(uint8_t* peripheralList, int position){
    return position;
}


        
/*TODO This is a hack and should not be here.
 * It is needed because the mapping from 8P line
 * to the device is not done properly.
 */
int EmptyDeviceModule::getValueForAddr(char addr){ return 0; }

/*
 *
 *
 */

#include "device.h"

#include <Arduino.h>

Device::Device(int address, int type):
    _address(address), _type(type)
{
    
}

void Device::loop(){
    Serial.println("Device Loop");
}

int Device::address(){
    return _address;
}

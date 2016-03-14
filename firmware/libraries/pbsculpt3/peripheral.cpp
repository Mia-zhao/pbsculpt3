/*
 *
 *
 */

#include "peripheral.h"

#include <Arduino.h>

Peripheral::Peripheral(int address, int type):
    _address(address), _type(type)
{
}

void Peripheral::loop(){
    //Serial.println("Device Loop");
}

int Peripheral::address(){
    return _address;
}

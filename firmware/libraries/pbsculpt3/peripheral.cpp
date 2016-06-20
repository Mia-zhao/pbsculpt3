/*
 *
 *
 */

#include "peripheral.h"

#include <Arduino.h>

Peripheral::Peripheral(int address, int type, int pin, bool fastPWM):
    _address(address), _type(type), _pin(pin), _fastPWM(fastPWM),
	mode(ACTIVE), time(0)
{
	events = LinkedList<PeripheralEvent>();
}

void Peripheral::loop(){
    //Serial.println("Device Loop");
}

int Peripheral::address(){
    return _address;
}

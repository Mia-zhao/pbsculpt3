/*
 *
 *
 */

#include "peripheral.h"

#include <Arduino.h>

#define MAX_EVENTS 10

Peripheral::Peripheral(int address, int type, char port, int pin, bool fastPWM):
    _address(address), _type(type), _port(port), _pin(pin), _fastPWM(fastPWM),
	mode(ACTIVE), time(0)
{
	events = LimitedQueue<PeripheralEvent>(MAX_EVENTS);
}

void Peripheral::loop(){
    //Serial.println("Device Loop");
}

int Peripheral::address(){
    return _address;
}

void Peripheral::handleLocalNeighbourBackgroundActivation(){}
void Peripheral::handleNeighbourBackgroundActivation(){}

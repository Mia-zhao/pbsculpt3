/*
 *
 *
 */

#include "peripheral.h"

#include "tools.h"

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

void Peripheral::handleDeviceNeighbourBackgroundActivation(PeripheralEvent e){}
void Peripheral::handleNodeNeighbourBackgroundActivation(PeripheralEvent e){}
void Peripheral::handleNeighbourBackgroundActivation(PeripheralEvent e){}

void Peripheral::startReflexBehaviour(){
	DBGLN("Peripheral", "Starting Reflex Behaviour.")
}


void Peripheral::switchMode(PeripheralMode newMode){
	if( mode == newMode ){
		return;
	}

	EventType evtType;

	switch(newMode){
	case PeripheralMode::OFF:
		evtType = EventType::OffMode;
		switchModeOff();
		mode = PeripheralMode::OFF;
		break;
	case PeripheralMode::TEST:
		evtType = EventType::TestMode;
		switchModeTest();
		mode = PeripheralMode::TEST;
		break;
	case PeripheralMode::SILENT:
		evtType = EventType::SilentMode;
		switchModeSilent();
		mode = PeripheralMode::SILENT;
		break;
	case PeripheralMode::ACTIVE:
		evtType = EventType::ActiveMode;
		switchModeActive();
		mode = PeripheralMode::ACTIVE;
		break;
	case PeripheralMode::BACKGROUND:
		evtType = EventType::BackgroundMode;
		switchModeBackground();
		mode = PeripheralMode::BACKGROUND;
		break;
	case PeripheralMode::SLAVE:
		evtType = EventType::SlaveMode;
		switchModeSlave();
		mode = PeripheralMode::SLAVE;
		break;
	}

	PeripheralEvent modeswitch = getEvent(evtType, time, _type, _port, _address);
	events.add(modeswitch);
}

void Peripheral::switchModeOff(){}
void Peripheral::switchModeTest(){}
void Peripheral::switchModeSilent(){}
void Peripheral::switchModeActive(){}
void Peripheral::switchModeBackground(){}
void Peripheral::switchModeSlave(){}

PeripheralEvent Peripheral::getEvent(EventType type, long time, int PeripheralType, char port, int address){
	return {type, time, PeripheralType, port, address};
}

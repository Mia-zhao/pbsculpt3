/*
 *
 *
 */

#include "deviceModule.h"

#include "pindefs.h"
#include "tools.h"
#include "peripheral.h"

#include <Arduino.h>

#define _SLOW_ONLY_PORT_1 3
#define _SLOW_ONLY_PORT_2 6

/**DeviceModules are the main units connected via 8P Modular cable to a Node
 *
 * DeviceModules can communicate over I2C, 4 PWM lines and 2 Analog lines to
 * the node. They can host peripherals and perform functionality.
 *
 * @param port The 1-indexed port number. For the current hardware, will be 1-6
 *
 */
DeviceModule::DeviceModule(char port):
    _port(port),
	_fpwm(&(FPWM_pin[(int)port-1][0])),
	_spwm(&(SPWM_pin[(int)port-1][0])),
	_analog(&(Analog_pin[(int)port-1][0]))
{
    peripherals = LinkedList<Peripheral*>();
}

void DeviceModule::loop(){
	for(int i=0; i<peripherals.size(); i++){
		while(peripherals.get(i)->events.size() > 0){
			PeripheralEvent event = peripherals.get(i)->events.pop();
			DBGLN("DM", "Pushing event.")
			switch(event.type){
			default:
				events.push(event);
				break;
			}
		}
	}
}

bool DeviceModule::fade(int address, int target, int duration){
    return false;
}

int  DeviceModule::read(int address, int preprocessType){
    return 0;
}

int DeviceModule::peripheralCount(){
    //Serial.printf("Port %i: %i devices...\n", _port, devices.size());
    return peripherals.size();
}

/**Does the module have access to fast PWM outputs?
 *
 * This is used to determine whether the outputs are FPWM 0-1 and SPWM 0-1
 * (true) or SPWM 0-3 (false).
 *
 */
bool DeviceModule::hasFastPWM(){
	// Ports 2 and 5 have only SPWM outputs
	if( _port==_SLOW_ONLY_PORT_1 || _port==_SLOW_ONLY_PORT_2 ){
		return false;
	}

	return true;
}

/**Does this address have access to FastPWM?
 *
 * If the address is 1-2 and the port has fast outputs, then yes
 *
 */
bool DeviceModule::hasFastPWMForAddress(int address){
	// Ports 2 and 5 have only SPWM outputs
	if( hasFastPWM() && address <= 2 ){
		return true;
	}

	return false;
}

/**What is the Arduino/Teensy pin number of this address?
 *
 * @param address The line number within the 8P cable.
 *
 */
int DeviceModule::getPinForAddress(int address){
	if (address >= 5 && address <= 6){
			DBGLN("DM", "Is a sensor...")
			return _analog[address-5];
	} else if( hasFastPWM() ){
		if( hasFastPWMForAddress(address) ){
			DBGLN("DM", "Has FPWM for Addr...")
			return _fpwm[address-1];
		} else {
			DBGLN("DM", "Has FPWM but not for Addr...")
			return _spwm[address-3];
		}
	} else if (address <= 4){
		DBGLN("DM", "Has no FPWM...")
		return _spwm[address-1];
	}

	return -1; // Is this right? Something's wrong.
}

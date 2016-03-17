/*
 *
 *
 */

#include "deviceModule.h"

#include "pindefs.h"
#include "peripheral.h"

#include <Arduino.h>

DeviceModule::DeviceModule(char port):
    _port(port),
	_fpwm(&(FPWM_pin[(int)port][0])),
	_spwm(&(SPWM_pin[(int)port][0])),
	_analog(&(Analog_pin[(int)port][0]))
{
    peripherals = LinkedList<Peripheral*>();
}

/*void SubNode::loop(){

    //std::map<int,Device>::iterator iter;
    
    for (iter = devices.begin(); iter != devices.end(); iter++) {
        //Switch to device port
        iter->second.loop();
    }
    
    int n = devices.size();
    for(int i=0; i<n; i++){
        devices[i].loop();
    }
}*/

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
	if( _port==2 || _port==5 ){
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
	if( hasFastPWM() ){
		if( hasFastPWMForAddress(address) ){
			return _fpwm[address-1];
		} else {
			return _spwm[address-3];
		}
	} else if (address <= 4){
		return _spwm[address-1];
	} else if (address <= 6){
		return _analog[address-5];
	}

	return -1; // Is this right? Something's wrong.
}

/*
 *
 *
 */

#define DEBUG 1

#include "node.h"

#include "Arduino.h"
#include "pindefs.h"
#include "tools.h"
#include "deviceModule.h"

//#include "wave_table.h"
#include "i2c_t3.h"

Node::Node(int serialNumber)
{
    //this->_serialNumber = serialNumber;
}

void Node::init(){
	DBGLN("Initializing Node.");

    // Call init for all of the devices
    for(int i=0; i < N_DEVICES; i++){
        devices[i]->init();
    }
    
    // All of Matt's init code:
    //===============================================
	//==== pin initialization ====
	//===============================================

	uint8_t num_ports = 0;
	uint8_t num_pins = 0;
	
	//--- Teensy On-Board ---
	pinMode(LED_BUILTIN, OUTPUT);
	
	//--- Analogue settings ---
	analogReadResolution(12);
	analogReadAveraging(32);
	analogWriteResolution(8);
	analogWriteFrequency(0, 1600);
	analogWriteFrequency(1, 1600);
	analogWriteFrequency(2, 1600);
	
	//--- Slow PWM driver ----
	//spwm = PWMDriver(0x40);

	//--- Multiplexer pins ---
	num_pins = sizeof(I2C_MUL_ADR_pin)/sizeof(I2C_MUL_ADR_pin[0]);
	for (uint8_t i = 0; i<num_pins; i++){
		pinMode(I2C_MUL_ADR_pin[i], OUTPUT);
	}	

	DBGLN("Starting I2C.");
	//--- I2C initialization ----
	Wire.begin(I2C_MASTER,0x00, I2C_PINS_18_19, I2C_PULLUP_EXT, I2C_RATE_400);

	DBGLN("Setting PWM Frequency.");
	noInterrupts();
	spwm.begin();
	spwm.setPWMFreq(1000);  // This is the maximum PWM frequency
	interrupts();

	DBGLN("Finished Initializing Node.");
}

void Node::loop(){

	// Make sure all of the device's loops are called
	for(int port0=0; port0 < N_DEVICES; port0++){
		devices[port0]->loop();
	}
}

int Node::deviceCount(){
	return N_DEVICES;
}

int Node::peripheralCount(){
    
    int count = 0;
    for(int i=0; i<N_DEVICES; i++){
        count += devices[i]->peripheralCount();
    }
    
    return count;
}

void Node::switchPort(char port){
    
}

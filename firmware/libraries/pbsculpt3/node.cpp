/*
 *
 *
 */

#include "node.h"
#include "deviceModule.h"

//#include "wave_table.h"
#include "i2c_t3.h"
#include "Arduino.h"

Node::Node(int serialNumber): /* 
    _serialNumber(serialNumber), devices(new SubNode[6])*/
    FPWM_pin {FPWM_1_pin, FPWM_2_pin, FPWM_3_pin, FPWM_4_pin, FPWM_5_pin, FPWM_6_pin},
	SPWM_pin {SPWM_1_pin, SPWM_2_pin, SPWM_3_pin, SPWM_4_pin, SPWM_5_pin, SPWM_6_pin}, 
	Analog_pin {Analog_1_pin, Analog_2_pin, Analog_3_pin, Analog_4_pin, Analog_5_pin, Analog_6_pin}
{
    //this->_serialNumber = serialNumber;
}

void Node::init(){
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
	
	//--- FPWM pins ---
	num_ports = sizeof(FPWM_pin)/sizeof(FPWM_pin[0]);
	for (uint8_t j = 0; j<num_ports; j++){
		num_pins = sizeof(FPWM_pin[j])/sizeof(FPWM_pin[j][0]);
		for (uint8_t i = 0; i<num_pins; i++){
			pinMode(FPWM_pin[j][i], OUTPUT);
		}
	}
	
	//--- Analogue pins ---
	num_ports = sizeof(Analog_pin)/sizeof(Analog_pin[0]);
	for (uint8_t j = 0; j<num_ports; j++){
		num_pins = sizeof(Analog_pin[j])/sizeof(Analog_pin[j][0]);
		for (uint8_t i = 0; i<num_pins; i++){
			pinMode(Analog_pin[j][i], INPUT);
		}	
	}
	
	//--- Analogue settings ---
	analogReadResolution(12);
	analogReadAveraging(32);
	analogWriteResolution(8);
	analogWriteFrequency(0, 1600);
	analogWriteFrequency(1, 1600);
	analogWriteFrequency(2, 1600);
	
	//--- Slow PWM driver ----
	spwm = PWMDriver(0x40);

	//--- Multiplexer pins ---
	num_pins = sizeof(I2C_MUL_ADR_pin)/sizeof(I2C_MUL_ADR_pin[0]);
	for (uint8_t i = 0; i<num_pins; i++){
		pinMode(I2C_MUL_ADR_pin[i], OUTPUT);
	}	

	//--- I2C initialization ----
	Wire.begin(I2C_MASTER,0x00, I2C_PINS_18_19, I2C_PULLUP_EXT, I2C_RATE_400);
	
	noInterrupts();
	spwm.begin();
	spwm.setPWMFreq(1000);  // This is the maximum PWM frequency
	interrupts();

}

void Node::loop(){
    // Call loop for all of the devices
    for(int port0=0; port0 < N_DEVICES; port0++){
        // Switch to port. Eventually move this to the subnode level.
        
        devices[port0]->loop();
        
        // WARNING: This is a super hack!
		uint8_t output_pins[4];
    	if (port0==2 || port0==5){
		    output_pins[0] = SPWM_pin[port0][0];
		    output_pins[1] = SPWM_pin[port0][1];
		    output_pins[2] = SPWM_pin[port0][2];
		    output_pins[3] = SPWM_pin[port0][3];
	    }
	    else{
		    output_pins[0] = FPWM_pin[port0][0];
		    output_pins[1] = FPWM_pin[port0][1];
		    output_pins[2] = SPWM_pin[port0][0];
		    output_pins[3] = SPWM_pin[port0][1];
	    }
        
        
        int addr1 = 1;
        int value = devices[port0]->getValueForAddr(addr1);
        /* MATT's CODE */
        /*if (addr1 >= 0 && addr1 < 4){
		    if (i==0 || i==1 || i==3 || i==4) {
			    if (addr1 < 2){*/
        /*if( i == 5 ){
        Serial.printf("Writing Port%i:%i (Teensy Pin %i) as %i\n", i+1, addr1, output_pins[addr1-1], value);
        }*/
	    if( port0 != 5 ){
	        analogWrite(output_pins[addr1-1], value);//addr1], level);
	    } else {
		    noInterrupts();
		    spwm.setPWMFast(output_pins[addr1-1], 16*value);
		    interrupts();
	    }
	    
			    /*}
			    else{
				    noInterrupts();
				    teensy_unit.spwm.setPWMFast(output_pins[addr1], 16*level);
				    interrupts();
			    }
		    }
	    }/* else {
		    noInterrupts();
		    teensy_unit.spwm.setPWMFast(output_pins[addr1], 16*level);
		    interrupts();
	    }*/
    }
}

int Node::deviceCount(){
    
    int count = 0;
    for(int i=0; i<sizeof(devices); i++){
        count += devices[i]->peripheralCount();
    }
    
    return count;
}

void Node::switchPort(char port){
    
}

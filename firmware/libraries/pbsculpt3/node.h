/* Represents a Teensy Node in Series 3
 *
 * author: David Kadish (david.kadish@gmail.com)
 * created: Feb 13, 2016
 *
 */

#ifndef __NODE_H
#define __NODE_H

#define N_SUBNODES 6

#include "subnode.h"

#include <Arduino.h>
#include "PWMDriver.h"

class Node {
    
    public:
        
        // Constructor
        Node(int serialNumber);
        
        void init();
        void loop();
        
        SubNode *subnodes[N_SUBNODES];
        
    protected:
        void switchPort(char port);
        
        int _serialNumber;
        
        
		PWMDriver spwm;	
		void spwm_init(uint16_t freq=1000);
        
		//--- Fast PWM pins ---
		const uint8_t FPWM_1_pin[2] = {3, 4};
		const uint8_t FPWM_2_pin[2] = {6, 5};
		const uint8_t FPWM_3_pin[0] = {};
		const uint8_t FPWM_4_pin[2] = {20, 21};
		const uint8_t FPWM_5_pin[2] = {25, 32};
		const uint8_t FPWM_6_pin[0] = {};
		const uint8_t* FPWM_pin[6];
		
		//--- Analogue pins ---
		const uint8_t Analog_1_pin[2] = {A11, A13};
		const uint8_t Analog_2_pin[2] = {A12, A15};
		const uint8_t Analog_3_pin[2] = {A16, A17};
		const uint8_t Analog_4_pin[2] = {A8, A9};
		const uint8_t Analog_5_pin[2] = {A2, A3};
		const uint8_t Analog_6_pin[2] = {A0, A1};
		const uint8_t* Analog_pin[6];
		
		//--- Slow PWM pin ----
		const uint8_t SPWM_1_pin[2] = {0, 1};
		const uint8_t SPWM_2_pin[2] = {2, 3};
		const uint8_t SPWM_3_pin[4] = {4, 5, 6, 7};
		const uint8_t SPWM_4_pin[2] = {14, 15};
		const uint8_t SPWM_5_pin[2] = {12, 13};
		const uint8_t SPWM_6_pin[4] = {8, 9 , 10, 11};
		const uint8_t* SPWM_pin[6];
		
		//--- I2C Multiplexer pins ---
		const uint8_t I2C_MUL_ADR_pin[3] = {2, 24, 33};
		const uint8_t I2C_MUL_ADR[6] = {4, 6, 7, 2, 1, 0};

};

#endif

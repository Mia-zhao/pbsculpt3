/*
 * pindefs.h
 *
 *  Created on: Mar 14, 2016
 *      Author: dkadish
 */

#ifndef __PINDEFS_H_
#define __PINDEFS_H_

#include "PWMDriver.h"

extern PWMDriver spwm;

#include <pins_arduino.h>

//--- Fast PWM pins ---
const static uint8_t FPWM_1_pin[2] = {3, 4};
const static uint8_t FPWM_2_pin[2] = {6, 5};
const static uint8_t FPWM_3_pin[0] = {};
const static uint8_t FPWM_4_pin[2] = {20, 21};
const static uint8_t FPWM_5_pin[2] = {25, 32};
const static uint8_t FPWM_6_pin[0] = {};
const static uint8_t* FPWM_pin[6] = {FPWM_1_pin, FPWM_2_pin, FPWM_3_pin, FPWM_4_pin, FPWM_5_pin, FPWM_6_pin};
//--- Analogue pins ---
const static uint8_t Analog_1_pin[2] = {A11, A13};
const static uint8_t Analog_2_pin[2] = {A12, A15};
const static uint8_t Analog_3_pin[2] = {A16, A17};
const static uint8_t Analog_4_pin[2] = {A8, A9};
const static uint8_t Analog_5_pin[2] = {A2, A3};
const static uint8_t Analog_6_pin[2] = {A0, A1};
const static uint8_t* Analog_pin[6] = {Analog_1_pin, Analog_2_pin, Analog_3_pin, Analog_4_pin, Analog_5_pin, Analog_6_pin};

//--- Slow PWM pin ----
const static uint8_t SPWM_1_pin[2] = {0, 1};
const static uint8_t SPWM_2_pin[2] = {2, 3};
const static uint8_t SPWM_3_pin[4] = {4, 5, 6, 7};
const static uint8_t SPWM_4_pin[2] = {14, 15};
const static uint8_t SPWM_5_pin[2] = {12, 13};
const static uint8_t SPWM_6_pin[4] = {8, 9, 10, 11};
const static uint8_t* SPWM_pin[6] = {SPWM_1_pin, SPWM_2_pin, SPWM_3_pin, SPWM_4_pin, SPWM_5_pin, SPWM_6_pin};

//--- I2C Multiplexer pins ---
const uint8_t I2C_MUL_ADR_pin[3] = {2, 24, 33};
const uint8_t I2C_MUL_ADR[6] = {4, 6, 7, 2, 1, 0};

#endif /* __PINDEFS_H_ */

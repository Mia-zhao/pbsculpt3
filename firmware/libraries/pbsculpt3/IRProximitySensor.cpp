/*
 * IRProximitySensor.cpp
 *
 *  Created on: Jun 8, 2016
 *      Author: dkadish
 */

#include <IRProximitySensor.h>

#include <Arduino.h>

IRProximitySensor::IRProximitySensor(int address, char port, int pin, bool fastPWM) :
	Peripheral(address, DEVICE_TYPE_PHOTO_SENSOR, port, pin, fastPWM),
	__capacitiveActivation(__IR_PROX_ACTIVATION_THRESHOLD){

}

IRProximitySensor::~IRProximitySensor() {

}

void IRProximitySensor::init(){
	pinMode(_pin, INPUT);
}

void IRProximitySensor::loop(){
	__capacitiveActivation.loop();
	__capacitiveActivation.addReading(analogRead(_pin));

	if(__capacitiveActivation.isActivated()){
		PeripheralEvent e = {Activation,(long)time,_type,_port,_address};
		events.push(e);
	}
}

int IRProximitySensor::fade(int target, int duration){
    return 0;
}

/**
 *
 * @todo Create proper framework for processing input data for sensors
 */
int IRProximitySensor::read(int preprocessType){
    if( preprocessType != 0 ){
        // Error invalid PreprocessingType
        return -1;
    }

    return analogRead(_pin);
}

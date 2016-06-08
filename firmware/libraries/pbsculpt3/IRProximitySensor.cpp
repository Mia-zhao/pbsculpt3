/*
 * IRProximitySensor.cpp
 *
 *  Created on: Jun 8, 2016
 *      Author: dkadish
 */

#include <IRProximitySensor.h>

#include <Arduino.h>

IRProximitySensor::IRProximitySensor(int address, int pin, bool fastPWM) :
Peripheral(address, DEVICE_TYPE_PHOTO_SENSOR, pin, fastPWM){

}

IRProximitySensor::~IRProximitySensor() {

}

void IRProximitySensor::init(){
	pinMode(_pin, INPUT);
}

void IRProximitySensor::loop(){
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

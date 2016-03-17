/*
 * AmbientLightSensor.cpp
 *
 *  Created on: Mar 14, 2016
 *      Author: dkadish
 */

#include <AmbientLightSensor.h>

#include <Arduino.h>

AmbientLightSensor::AmbientLightSensor(int address, int pin, bool fastPWM) :
	Peripheral(address, DEVICE_TYPE_PHOTO_SENSOR, pin, fastPWM){

}

void AmbientLightSensor::init(){
	pinMode(_pin, INPUT);
}

void AmbientLightSensor::loop(){
}

int AmbientLightSensor::fade(int target, int duration){
    return 0;
}

int AmbientLightSensor::read(int preprocessType){
    if( preprocessType != 0 ){
        // Error invalid PreprocessingType
        return -1;
    }

    return analogRead(_pin);
}

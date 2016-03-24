/*
 * Fin.cpp
 *
 *  Created on: Mar 24, 2016
 *      Author: dkadish
 *
 *  Defines methods for controlling a fin from the WNA Washington Project.
 */

#include <FinSMA.h>
#include "pindefs.h"

FinSMA::FinSMA(int address, int pin, bool fastPWM) :
Peripheral(address, 3, pin, fastPWM), _value(0.0), _fadeInitValue(0.0), _fadeDuration(0),
    _fadeTime(0), _onTime(0), _offTime(0), _fadeTarget(0), _max_sma_on_time(__MAX_SMA_ON_TIME),
	_min_sma_off_time(__MIN_SMA_OFF_TIME), _max_sma_level(__MAX_SMA_LEVEL), _lockout(false)
{

}

void FinSMA::init(){ //TODO: Break this out into the base class
	if(_fastPWM){
		pinMode(_pin, OUTPUT);
	}
}

void FinSMA::loop(){
	// Do we need to switch the status of the lockout?
	if( _onTime > _max_sma_on_time ){
		_value = 0;
		_offTime = 0;
		_lockout = true;
	} else if (_lockout && _offTime > _min_sma_off_time){
		// Undo the lockout
		_lockout = false;
	}

    // Adjust the value as we fade towards the next step
	if( !_lockout ){
		if( _fadeDuration > _fadeTime ){
			float slope = ((float)_fadeTarget - _fadeInitValue)/((float)_fadeDuration);
			_value = _fadeInitValue + slope * (float)_fadeTime;
			//Serial.printf("FADING: %f, %i, %f, %f, %i, %i\n", _value, _fadeTarget, _fadeInitValue, slope, _fadeDuration, (int)_fadeTime);
		} else {
			_value = (float)_fadeTarget;
		}
	}

	// Reset on counter if it is off
	if (_value == 0){
		_onTime = 0;
	}

	// Push the value to the actuator
    if( _fastPWM ){
    	analogWrite(_pin, map(_value, 0, 255, 0, _max_sma_level));
    } else {
    	noInterrupts();
		spwm.setPWMFast(_pin, map(_value, 0, 255, 0, 16*_max_sma_level));
		interrupts();
    }
}

/**
 *
 * @param target Integer from 0-255 with the fade target value. Will be scaled automatically to the limits of the SMA.
 * @param duration Duration of the fade in milliseconds
 * @return 0 for success
 */
int FinSMA::fade(int target, int duration){
    _fadeDuration = duration;
    _fadeTime = 0;
    _fadeTarget = target;
    _fadeInitValue = _value;

    return 0;
}

int FinSMA::read(int preprocessType){
    if( preprocessType != 0 ){
        // Error invalid PreprocessingType
        return -1;
    }

    return (int)_value;
}

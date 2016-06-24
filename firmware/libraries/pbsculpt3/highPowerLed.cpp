/*
 *
 *
 */

#include "highPowerLed.h"
#include "pindefs.h"
#include "tools.h"

#include <Arduino.h>

#define __TESTING__ 0

#if __TESTING__
#define MEAN 2500L
#define STD 5000L
#define INACTIVITY_THRESHOLD 5000L
#else
#define MEAN 15000L
#define STD 30000L
#define INACTIVITY_THRESHOLD 50000L
#endif
#define ACCUMULATION_INTERVAL 1

#define LOCAL_BG_ACTIVITY_INHIBITION_FACTOR 2
#define COMMUNAL_BG_ACTIVITY_INHIBITION_FACTOR 10

HighPowerLED::HighPowerLED(int address, char  port, int pin, bool fastPWM) :
Peripheral(address, 3, port, pin, fastPWM), _value(0.0), _fadeInitValue(0.0), _fadeDuration(0),
    _fadeTime(0), _fadeTarget(0), _inactivityThreshold(INACTIVITY_THRESHOLD),
	_accumulationInterval(ACCUMULATION_INTERVAL),
	_acc_rate(2), _red_rate(1), _rand(0), _randGenerator(GaussianRandom(MEAN, STD))
{
    // Set up background behaviour
    const int n_timeValue = 18;
    unsigned long timeValue[n_timeValue][2] = {
    		{0L, 0L},
    		{100000L, 30L},
    		{200000L, 86L},
    		{300000L, 134L},
    		{400000L, 174L},
    		{500000L, 206L},
    		{600000L, 230L},
    		{700000L, 246L},
    		{800000L, 254L},
    		{900000L, 254L},
    		{1000000L, 246L},
    		{1100000L, 230L},
    		{1200000L, 206L},
    		{1300000L, 174L},
    		{1400000L, 134L},
    		{1500000L, 86L},
    		{1600000L, 30L},
    		{1700000L, 0L}
    };
    _backgroundBehaviour = Behaviour();

	for( int i = 0; i < n_timeValue; i++ ){
		Point *point = new Point();
		point->time = timeValue[i][0];
		point->value = timeValue[i][1];

		_backgroundBehaviour.points.add(point);
	}
}

void HighPowerLED::init(){
	if(_fastPWM){
		pinMode(_pin, OUTPUT);
	}
}

/**
 * @todo Create background behaviour
 */
void HighPowerLED::loop(){
    switch(mode){
	case ACTIVE:
		//Serial.println("HighPowerLED Loop");
		// Adjust the value as we fade towards the next step
		if( _fadeDuration > _fadeTime ){
			float slope = ((float)_fadeTarget - _fadeInitValue)/((float)_fadeDuration);
			_value = _fadeInitValue + slope * (float)_fadeTime;
			//Serial.printf("FADING: %f, %i, %f, %f, %i, %i\n", _value, _fadeTarget, _fadeInitValue, slope, _fadeDuration, (int)_fadeTime);
		} else {
			_value = (float)_fadeTarget;
		}

		// Reset the inactivity timer if there is activity.
		if( _value > 0 ){
			inactivityTimer = 0;
		}

		break;
	case BACKGROUND:
		_backgroundBehaviour.loop();
		_value = _backgroundBehaviour.value();

		break;
    }

    backgroundBehaviour();

    _setPinToValue();
}

int HighPowerLED::fade(int target, int duration){
    _fadeDuration = duration;
    _fadeTime = 0;
    _fadeTarget = target;
    _fadeInitValue = _value;
    
    //Serial.println("Got Fade Command.");
    
    return 0;
}

int HighPowerLED::read(int preprocessType){
    if( preprocessType != 0 ){
        // Error invalid PreprocessingType
        return -1;
    }
    
    return (int)_value;
}

void HighPowerLED::backgroundBehaviour(){

	switch(mode){
	case ACTIVE:
		if( inactivityTimer > _inactivityThreshold){
			DBGLN("HighPowerLED", "Switching to Background mode...")
			_switchToBackgroundMode();
		}

		break;
	case BACKGROUND:
		// Check to make sure that enough time has passed AND no background is playing
		if( _accumulationTimer >= _accumulationInterval && !_backgroundBehaviour.isPlaying()){
			_accumulationTimer = 0; // Reset timer
			_accumulator += _acc_rate - _red_rate; // Accumulate and reduce
			if( _accumulator > _rand ){ // Start the background activation event
				DBGF("HighPoweredLED", "Starting background behaviour (%d)...",  _rand)
				_startBackgroundActivation();
			}
		}

		break;
	case TEST:
	case SILENT:
	case OFF:
	case SLAVE:
		break;
	}
}

void HighPowerLED::_setPinToValue(){
	if( _fastPWM ){
		analogWrite(_pin, _value);
	} else {
		noInterrupts();
		spwm.setPWMFast(_pin, 16*_value);
		interrupts();
	}
}

/** Switch to background mode. Only called if not in background mode already.
 *
 */
void HighPowerLED::_switchToBackgroundMode(){
	if( mode != BACKGROUND ){
		mode = BACKGROUND;
		_accumulator = 0; // Reset accumulator
		_accumulationTimer = 0; // Reset timer
		_rand = _randGenerator.randLong();

		PeripheralEvent e = {BackgroundMode,time,_type,_port,_address};
		DBGLN("HighPowerLED", "Background event created...")

		events.push(e);
		DBGLN("HighPowerLED", "Background event pushed...")
	}
}

void HighPowerLED::_startBackgroundActivation(){
	// Create background lighting
	_backgroundBehaviour.play();
	_accumulator = 0; // Reset accumulator

	// Reset the random number
	_rand = _randGenerator.randLong();

	// Log the event
	PeripheralEvent e = {BackgroundActivation,(long)time,_type,_port,_address};
	events.push(e);
}

void HighPowerLED::handleLocalNeighbourBackgroundActivation(){
	switch (mode) {
		case BACKGROUND:
			//if(!_backgroundBehaviour.isPlaying()){
				DBGF("HighPoweredLED", "Local Bg Activation (%d-%d): Reducing accumulator from %d to %d",
						_port,_address,_accumulator, _accumulator-_accumulator / LOCAL_BG_ACTIVITY_INHIBITION_FACTOR)
				// Increase the time to activation if a local neighbour goes off
				_accumulator -= _accumulator / LOCAL_BG_ACTIVITY_INHIBITION_FACTOR;
			//}
			break;
		default:
			break;
	}
}

void HighPowerLED::handleNeighbourBackgroundActivation(){
	switch (mode) {
		case BACKGROUND:
			DBGF("HighPoweredLED", "Bg Activation: Reducing accumulator from %d to %d",
					_accumulator, _accumulator-_accumulator / COMMUNAL_BG_ACTIVITY_INHIBITION_FACTOR)
			// Increase the time to activation if any neighbour goes off
			_accumulator -= _accumulator / COMMUNAL_BG_ACTIVITY_INHIBITION_FACTOR;
			break;
		default:
			break;
	}
}

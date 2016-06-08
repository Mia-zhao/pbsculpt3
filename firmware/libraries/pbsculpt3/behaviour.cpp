/*
 * behaviour.cpp
 *
 *  Created on: May 27, 2016
 *      Author: dkadish
 */

#include "behaviour.h"
#include <LinkedList.h>

#if DEBUG_BEHAVIOUR
	#include "Arduino.h"
#endif

Behaviour::Behaviour():
	_runTime(0),
	_pauseTime(0),
	_maxValue(255),
	_timeMux(1),
	_timeDiv(1),
	_valueMux(1),
	_valueDiv(1),
	_state(STOPPED),
	_i_lastPoint(0),
	_lastPoint(0),
	_nextPoint(0)
	{

    points = LinkedList<Point*>();
}

Behaviour::~Behaviour() {
}

void Behaviour::loop(){
	switch(_state){
	case PLAYING:
		// Check to see if we should move forward to the next control point?
		if( _runTime > nextPointStartTime() ){
			// Move forward one point, but if this is the end, break.
			if(!advancePoint()){
				_state = STOPPED;
			}
		}
		break;
	}
#if DEBUG_BEHAVIOUR > 2
	Serial.printf("BEHAVIOUR: Looping... Value at %d, State %d\n", (int)value(), (int)_state);
#endif
}

int Behaviour::value(){
	long val = 0L;
	switch(_state){
	case PLAYING:
	{
		long t_1 = _lastPoint->time;
		long t_2 = _nextPoint->time;
		long v_1 = (long) _lastPoint->value;
		long v_2 = (long) _nextPoint->value;
		long v_d = v_2 - v_1;
		long t_d = t_2 - t_1;
		long t_from_last = _runTime - t_1;
		val = v_1 + (v_d * t_from_last) / t_d;

#if DEBUG_BEHAVIOUR > 3
	Serial.printf("BEHAVIOUR: Value Difference %d - %d = %d\n", v_2, v_1, v_d);
	Serial.printf("BEHAVIOUR: Time Difference %d - %d = %d\n", t_2, t_1, t_d);
	Serial.printf("BEHAVIOUR: Value %d + ( %d * %d ) / %d = %d\n", v_1, v_d, t_from_last, t_d, val);
#endif
	}
	}

	if(val > _maxValue){
		val = _maxValue;
	}

	return val;
}

void Behaviour::play(){
	switch(_state){
	case STOPPED:
		_runTime = 0;
		_i_lastPoint = 0;
		loadPoints();
		break;
	case PAUSED:
		_runTime = _runTime - _pauseTime; // @TODO: test this
		break;
	}
	_state = PLAYING;
}

void Behaviour::stop(){
	_state = STOPPED;
}

void Behaviour::pause(){
	switch(_state){
	case PLAYING:
		_pauseTime = 0;
		break;
	case STOPPED:
		_runTime = 0;
		_i_lastPoint = 0;
	}
	_state = PAUSED;
}

void Behaviour::reset(){
	_runTime = 0;
	_i_lastPoint = 0;
	loadPoints();
}

// Set limitations for behaviour
void Behaviour::setMaxValue(unsigned long value){
	_maxValue = value;
}

// Set multipliers and dividers
void Behaviour::setTimeMultiplier(int mux){
	// @TODO: Implement a non-stopped version
	switch(_state){
	case STOPPED:
		_timeMux = mux;
	}
}

void Behaviour::setTimeDivisor(int div){
	// @TODO: Implement a non-stopped version
	switch(_state){
	case STOPPED:
		_timeDiv = div;
	}
}

void Behaviour::setValueMultiplier(int mux){
	_valueMux = mux;
}

void Behaviour::setValueDivisor(int div){
	_valueDiv = div;
}

unsigned long Behaviour::nextPointStartTime(){
#if DEBUG_BEHAVIOUR > 3
	Serial.printf(
			"BEHAVIOUR: Next point starts at %d microseconds (%d current, %d next time, %d last time, %d timeX, %dtimeoX)\n",
			(_nextPoint->time * _timeMux) / _timeDiv, (int) _runTime,
			_nextPoint->time, _lastPoint->time, _timeMux, _timeDiv
		);
	Serial.printf("BEHAVIOUR: %d | %d\n", _nextPoint->time, points.get(_i_lastPoint+1)[0]);
#endif
	return (_nextPoint->time * _timeMux) / _timeDiv;
}

unsigned long Behaviour::currentPointStartTime(){
	return (_lastPoint->time * _timeMux) / _timeDiv;
}

bool Behaviour::advancePoint(){

	_i_lastPoint += 1;

	bool success = loadPoints();

#if DEBUG_BEHAVIOUR > 2
	Serial.printf("BEHAVIOUR: Advance Called. i: %d/%d, success: %d.\n", _i_lastPoint, points.size(), (int)success);
#endif

	return success;
}

bool Behaviour::loadPoints(){
	bool success = false;
	if( _i_lastPoint + 1 < points.size() ){
		_lastPoint = points.get(_i_lastPoint);
		_nextPoint = points.get(_i_lastPoint+1);
		success = true;
	} else {
		_i_lastPoint = 0;
		_lastPoint = points.get(_i_lastPoint);
		if ( points.size() > 1 ){
			_nextPoint = points.get(_i_lastPoint+1);
		} else {
			_nextPoint = 0;
		}
		success = false;
	}

#if DEBUG_BEHAVIOUR > 1
	Serial.printf("BEHAVIOUR: Points Loaded : [%d: %d, %d: %d]\n", _lastPoint->time, _lastPoint->value, _nextPoint->time, _nextPoint->value);
#endif

	return success;
}

bool Behaviour::isPlaying(){
	return _state == PLAYING;
}

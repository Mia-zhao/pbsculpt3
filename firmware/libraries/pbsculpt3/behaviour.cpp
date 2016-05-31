/*
 * behaviour.cpp
 *
 *  Created on: May 27, 2016
 *      Author: dkadish
 */

#include "behaviour.h"

Behaviour::Behaviour():
	_runTime(0),
	_pauseTime(0),
	_maxValue(255),
	_timeMux(1),
	_timeDiv(1),
	_valueMux(1),
	_valueDiv(1),
	_state(STOPPED),
	_i_lastPoint(0)
	{

    points = LinkedList<Long*>();
}

Behaviour::~Behaviour() {
	// TODO Auto-generated destructor stub
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
}

int Behaviour::value(){
	switch(_state){
	case PLAYING:
		long t_1 = _lastPoint[0], t_2 = _nextPoint[0],
			 v_1 = _lastPoint[1], v_2 = _lastPoint[1];

		long v_d = v_2 - v_1, t_d = t_2 - t_1;

		return v_1 + v_d * (_runTime - t_1) / t_d;
	case STOPPED:
		return 0;
	case PAUSED:
		return 0;
	}
}

void Behaviour::play(){
	switch(_state){
	case STOPPED:
		_runTime = 0;
		_i_lastPoint = 0;
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

	}
	_state = PAUSED;
}

void Behaviour::reset(){
	_runTime = 0;
	_i_lastPoint = 0;
}

// Set limitations for behaviour
void Behaviour::setMaxValue(long value){
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

// @TODO: FINISH THIS!
bool Behaviour::advancePoint(){
	_i_lastPoint += 1;
	if( _i_lastPoint < points.size() ){
		_lastPoint = points.get(_i_lastPoint);
		_nextPoint = points.get(_i_lastPoint+1);
		return true;
	} else {
		_i_lastPoint = 0;
		_lastPoint = points.get(_i_lastPoint);
		if ( points.size() > 1 ){
			_nextPoint = points.get(_i_lastPoint+1);
		} else {
			_nextPoint = 0;
		}
		return false;
	}

}

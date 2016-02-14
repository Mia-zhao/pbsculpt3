/*
 *
 *
 */

#include "highPowerLed.h"
#include <Arduino.h>

HighPowerLED::HighPowerLED(int address) :
    Device(address,3), _value(0)
{
    
}

void HighPowerLED::init(){    
    _fadeDuration = 0;
    _fadeTime = 0;
    _fadeTarget = 0;
}

void HighPowerLED::loop(){
    Serial.println("HighPowerLED Loop");
    // Adjust the value as we fade towards the next step
    if( (_fadeDuration - _fadeTime) > 0 ){
        _value += (_value - _fadeTarget)/(_fadeDuration - _fadeTime);
    } // Else stay at the same value
}

int HighPowerLED::fade(int target, int duration){
    _fadeDuration = duration;
    _fadeTime = 0;
    _fadeTarget = target;
    
    return 0;
}

int HighPowerLED::read(int preprocessType){
    if( preprocessType != 0 ){
        // Error invalid PreprocessingType
        return -1;
    }
    
    return _value;
}

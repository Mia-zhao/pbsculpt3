/*
 *
 *
 */

#include "highPowerLed.h"
#include <Arduino.h>

HighPowerLED::HighPowerLED(int address) :
    Device(address,3), _value(0.0), _fadeInitValue(0.0), _fadeDuration(0),
    _fadeTime(0), _fadeTarget(0)
{
    
}

void HighPowerLED::init(){
}

void HighPowerLED::loop(){
    //Serial.println("HighPowerLED Loop");
    // Adjust the value as we fade towards the next step
    if( _fadeDuration > _fadeTime ){
        float slope = ((float)_fadeTarget - _fadeInitValue)/((float)_fadeDuration);
        _value = _fadeInitValue + slope * (float)_fadeTime;
        //Serial.printf("FADING: %f, %i, %f, %f, %i, %i\n", _value, _fadeTarget, _fadeInitValue, slope, _fadeDuration, (int)_fadeTime);
    } else {
        _value = (float)_fadeTarget;
    }
}

int HighPowerLED::fade(int target, int duration){
    _fadeDuration = duration;
    _fadeTime = 0;
    _fadeTarget = target;
    _fadeInitValue = _value;
    
    Serial.println("Got Fade Command.");
    
    return 0;
}

int HighPowerLED::read(int preprocessType){
    if( preprocessType != 0 ){
        // Error invalid PreprocessingType
        return -1;
    }
    
    return (int)_value;
}

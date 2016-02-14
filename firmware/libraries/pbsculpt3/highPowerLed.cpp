/*
 *
 *
 */

#include "highPowerLed.h"

HighPowerLED::HighPowerLED(int address) :
    Device(address,3), _value(0)
{
    
}

void HighPowerLED::init(){}

void HighPowerLED::loop(){
    // Adjust the value as we fade towards the next step
    _value += (_value - _fadeTarget)/(_fadeDuration - _fadeTime);
    
    // Send value to device
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

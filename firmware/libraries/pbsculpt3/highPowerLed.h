/* Represents a High Power LED for a submodule in Series 3
 *
 * author: David Kadish (david.kadish@gmail.com)
 * created: Feb 13, 2016
 *
 */

#ifndef __HIGHPOWERLED_H
#define __HIGHPOWERLED_H

#include "peripheral.h"
#include <elapsedMillis.h>

class HighPowerLED : public Peripheral {
    
    public:
        
        // Constructor
        HighPowerLED(int address, int pin, bool fastPWM);
        
        void init();
        void loop();
        
        int fade(int target, int duration);
        int read(int preprocessType);
        
        int value();
        
    protected:
        float _value, _fadeInitValue;
        
        // Fade Variables
        int _fadeDuration, _fadeTarget;
        elapsedMillis _fadeTime;
        
};

#endif

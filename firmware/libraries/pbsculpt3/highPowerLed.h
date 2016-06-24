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
#include "GaussianRandom.h"
#include "behaviour.h"

class HighPowerLED : public Peripheral {
    
    public:
        
        // Constructor
        HighPowerLED(int address, char port, int pin, bool fastPWM);
        
        void init();
        void loop();
        
        int fade(int target, int duration);
        int read(int preprocessType);
        
        int value();

        // Event handling system
        void handleLocalNeighbourBackgroundActivation();
        void handleNeighbourBackgroundActivation();
        
    protected:
        void _setPinToValue();
        void _switchToBackgroundMode();
        void _startBackgroundActivation();

        float _value, _fadeInitValue;
        
        // Fade Variables
        int _fadeDuration, _fadeTarget;
        elapsedMillis _fadeTime;
        
        /** Regulates the background behaviour of the LED
         *
         */
        void backgroundBehaviour();
        void resetAccumulator();

        long _accumulator;

        /** The rate of accumulation for the accumulator
         *
         */
        int _acc_rate;

        /** The rate of reduction of the accumulator
         *
         * Should be less than the accumulation rate to create background
         * behaviours.
         *
         */
        int _red_rate;

        elapsedMillis inactivityTimer;
        long _inactivityThreshold;

        elapsedMillis _accumulationTimer;
        long _accumulationInterval;

        long _rand;
        GaussianRandom _randGenerator;

        Behaviour _backgroundBehaviour;
};

#endif

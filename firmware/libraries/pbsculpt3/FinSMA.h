/*
 * Fin.h
 *
 *  Created on: Mar 24, 2016
 *      Author: dkadish
 */

#ifndef FIN_H_
#define FIN_H_

#define __MAX_SMA_ON_TIME 1000
#define __MIN_SMA_OFF_TIME 1000
#define __MAX_SMA_LEVEL 100

#include "peripheral.h"
#include "Arduino.h"

class FinSMA: public Peripheral {
public:
	FinSMA(int address, int pin, bool fastPWM);

	void init();
	void loop();

	int fade(int target, int duration);
	int read(int preprocessType);

	int value();

protected:
    float _value, _fadeInitValue;

    // Fade Variables
    int _fadeDuration, _fadeTarget;
    elapsedMillis _fadeTime, _onTime, _offTime;

    int _max_sma_on_time; // The maximum consecutive time on for the SMA in millis (NAS=~1 second)
    int _min_sma_off_time; // The rest time once an SMA has been engaged for the full length of time in millis
    int _max_sma_level; // The maximum PWM value for the SMA out of 255 (NAS=150)

    bool _lockout; // Is the power to the SMA locked out while we wait for it to cool?
};

#endif /* FIN_H_ */

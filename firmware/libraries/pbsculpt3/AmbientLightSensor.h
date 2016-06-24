/*
 * AmbientLightSensor.h
 *
 *  Created on: Mar 14, 2016
 *      Author: dkadish
 */

#ifndef AMBIENTLIGHTSENSOR_H_
#define AMBIENTLIGHTSENSOR_H_

#include <peripheral.h>

class AmbientLightSensor: public Peripheral {
	public:
		AmbientLightSensor(int address, char port, int pin, bool fastPWM);

		void init();
		void loop();

		int fade(int target, int duration);
		int read(int preprocessType);

		int value();
};

#endif /* AMBIENTLIGHTSENSOR_H_ */

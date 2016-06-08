/*
 * IRProximitySensor.h
 *
 *  Created on: Jun 8, 2016
 *      Author: dkadish
 */

#ifndef IRPROXIMITYSENSOR_H_
#define IRPROXIMITYSENSOR_H_

#include <peripheral.h>

class IRProximitySensor: public Peripheral {
public:
	IRProximitySensor(int address, int pin, bool fastPWM);
	virtual ~IRProximitySensor();

	void init();
	void loop();

	int fade(int target, int duration);
	int read(int preprocessType);
	int value();
};

#endif /* IRPROXIMITYSENSOR_H_ */

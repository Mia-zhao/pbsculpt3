/*
 * IRProximitySensor.h
 *
 *  Created on: Jun 8, 2016
 *      Author: dkadish
 */

#ifndef IRPROXIMITYSENSOR_H_
#define IRPROXIMITYSENSOR_H_

#include <peripheral.h>
#include "AdaptiveCapacitor.h"

#define __IR_PROX_ACTIVATION_THRESHOLD 100000

class IRProximitySensor: public Peripheral {
public:
	IRProximitySensor(int address, char port, int pin, bool fastPWM);
	virtual ~IRProximitySensor();

	void init();
	void loop();

	int fade(int target, int duration);
	int read(int preprocessType);
	int value();

private:

	data::AdaptiveCapacitor __capacitiveActivation;
};

#endif /* IRPROXIMITYSENSOR_H_ */

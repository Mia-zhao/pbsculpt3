/*
 * AdaptiveCapacitor.h
 *
 *  Created on: Jun 29, 2016
 *      Author: dkadish
 */

#ifndef ADAPTIVECAPACITOR_H_
#define ADAPTIVECAPACITOR_H_

#define TIME_BETWEEN_READINGS 10 // In ms. Approx 1/10 of human reaction time.
#define LONG_AVG_FADER 100 // The average is multiplied by (LAF-1)/LAF
#define SHORT_AVG_FADER 10

#include "tools.h"
#include <Metro.h> // Include the Metro library

namespace data {

class AdaptiveCapacitor {
public:

	/** Filters sensory data to trigger activations
	 *
	 *	The AdaptiveCapacitor imagines sensor readings as a bucket. The bucket
	 *	has an inflow rate and and outflow rate and is triggered when it is
	 *	filled to a pre-defined level (threshold). The inflow and outflow are
	 *	controlled by running averages of sensor data, the inflow being a
	 *	short-term average, and the outflow being a long-term average.
	 *	This allows the system to account for drift in the sensor values while
	 *	being reactive to short term triggerings of the sensor.
	 *
	 * @param threshold The level at which to trigger the sensor
	 */
	AdaptiveCapacitor(unsigned long threshold);
	virtual ~AdaptiveCapacitor();

	/** Loop is called once per Arduino cycle to register new readings and
	 * 	proces any data that is necessary.
	 *
	 */
	void loop();

	/** Check to see if the capacitor has been activated.
	 *
	 * Checks to see if the fill has risen above the threshold for activation.
	 * If so, returns true and also empties the capacitor, resetting the state.
	 *
	 * @return True if the fill is greater than the threshold
	 */
	bool isActivated();

	/**
	 *
	 * @param reading
	 */
	void addReading(int reading);

protected:

	/** Is it time to take another reading?
	 *
	 */
	Metro readMetro;

	/** The long term running average
	 *
	 */
	unsigned long long_avg;


	/** The short term running average
	 *
	 */
	unsigned long short_avg;

	/** The capacitive threshold
	 *
	 */
	unsigned long threshold;

	/** The fill level of the capacitive element
	 *
	 */
	unsigned long level;

	unsigned long readingSum, readingCount;

	bool firstReading;

};

} /* namespace data */

#endif /* ADAPTIVECAPACITOR_H_ */

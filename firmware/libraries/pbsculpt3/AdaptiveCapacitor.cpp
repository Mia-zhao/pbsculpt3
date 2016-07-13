/*
 * AdaptiveCapacitor.cpp
 *
 *  Created on: Jun 29, 2016
 *      Author: dkadish
 */

#include <AdaptiveCapacitor.h>

namespace data {

AdaptiveCapacitor::AdaptiveCapacitor(unsigned long threshold):readMetro(TIME_BETWEEN_READINGS),
		long_avg(0), short_avg(0), threshold(threshold), level(0), readingSum(0), readingCount(0),
		firstReading(true) {
	// TODO Auto-generated constructor stub
}

AdaptiveCapacitor::~AdaptiveCapacitor() {
	// TODO Auto-generated destructor stub
}

void AdaptiveCapacitor::loop(){
	// check if the metro has passed it's interval and if there are new readings.
	if (readMetro.check() == 1 && readingCount > 0) {

		//DBGF("AdaptCap", "Readings: %d / %d = %d", readingSum, readingCount, readingSum/readingCount)

		// Calculate the last reading
		unsigned long lastReading = readingSum/readingCount;
		readingSum = 0;
		readingCount = 0;

		if( firstReading ){
			long_avg = lastReading;
			short_avg = lastReading;
			firstReading = false;
		} else {
			// Update the running averages
			long_avg = ( (long_avg * (LONG_AVG_FADER-1)) + lastReading ) / LONG_AVG_FADER;
			short_avg = ( (short_avg * (SHORT_AVG_FADER-1)) + lastReading ) / SHORT_AVG_FADER;
		}

		level += short_avg;
		// Floor subtraction at 0
		if( level > long_avg + 1 ){
			level -= long_avg + 1;
		} else {
			level = 0;
		}

		//DBGF("AdaptCap", "Averages: Long = %d, Short = %d, Level = %d", long_avg, short_avg, level)
	}
}

bool AdaptiveCapacitor::isActivated(){
	if( level > threshold ){
		DBGF("AdaptCap", "Activation! Long: %d, Short %d", long_avg, short_avg)
		level = 0;
		return true;
	}

	return false;
}

void AdaptiveCapacitor::addReading(int reading){
	readingSum += reading;
	readingCount += 1;
}

} /* namespace data */

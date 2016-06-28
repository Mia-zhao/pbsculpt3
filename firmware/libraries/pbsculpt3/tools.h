/*
 * tools.h
 *
 *  Created on: Mar 18, 2016
 *      Author: dkadish
 */

#ifndef TOOLS_H_
#define TOOLS_H_

#include <Arduino.h>

#define DEBUG_DELAY 10

#ifdef DEBUG

#define DBG(msg) if (DEBUG){ Serial.print(msg); delay(DEBUG_DELAY); } // Was seizing up before message had a chance to print, so hence the delay ***MAKE A NOTE ABOUT THIS***
#define DBGLN(pre, msg) if (DEBUG){ Serial.print(pre); Serial.printf(" %d: ", __LINE__); Serial.println(msg); delay(DEBUG_DELAY); }
#define DBGF(pre, msg, args...) if (DEBUG){ Serial.print(pre); Serial.printf(" %d: ", __LINE__); Serial.printf(msg, args); Serial.println(); delay(DEBUG_DELAY); }

#else

#define DBG(msg)
#define DBGLN(pre,msg)
#define DBGF(pre,msg)

#endif

enum XBeeMessageType {
	Ping,
	Event,
	Status
};

#endif /* TOOLS_H_ */

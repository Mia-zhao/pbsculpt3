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

#define DBG(lvl, msg) if (DEBUG >= lvl){ Serial.print(msg); delay(DEBUG_DELAY); } // Was seizing up before message had a chance to print, so hence the delay ***MAKE A NOTE ABOUT THIS***
#define DBGLN(lvl, pre, msg) if (DEBUG >= lvl){ Serial.print(pre); Serial.printf(" %d: ", __LINE__); Serial.println(msg); delay(DEBUG_DELAY); }
#define DBGF(lvl, pre, msg, args...) if (DEBUG >= lvl){ Serial.print(pre); Serial.printf(" %d: ", __LINE__); Serial.printf(msg, args); Serial.println(); delay(DEBUG_DELAY); }

#else

#define DBG(lvl, msg) do {} while(0)
#define DBGLN(lvl, pre,msg) do {} while(0)
#define DBGF(lvl, pre,msg, args...) do {} while(0)

#endif

enum XBeeMessageType {
	Ping,
	Event,
	Status
};

#endif /* TOOLS_H_ */

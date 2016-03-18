/*
 * tools.h
 *
 *  Created on: Mar 18, 2016
 *      Author: dkadish
 */

#ifndef TOOLS_H_
#define TOOLS_H_

#include <Arduino.h>

#define DEBUG 1

#define DEBUG_DELAY 10

#define DBG(msg) if (DEBUG){ Serial.print(msg); delay(DEBUG_DELAY); } // Was seizing up before message had a chance to print, so hence the delay ***MAKE A NOTE ABOUT THIS***
#define DBGLN(msg) if (DEBUG){ Serial.println(msg); delay(DEBUG_DELAY); }

#endif /* TOOLS_H_ */

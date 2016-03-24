/*
 * FinReflexDM.h
 *
 *  Created on: Mar 24, 2016
 *      Author: dkadish
 */

#ifndef FINREFLEXDM_H_
#define FINREFLEXDM_H_

#define __RIGHT_SMA_ADDRESS 1
#define __LEFT_SMA_ADDRESS	3

#include "deviceModule.h"

#include <LinkedList.h>

class FinReflexDM: public DeviceModule {
public:
	FinReflexDM(char port);

	void init();
	void loop();

	bool fade(int address, int target, int duration);
	int  read(int address, int preprocessType);

	//
	int getPeripheralList(uint8_t* peripheralList, int position);        /*TODO This is a hack and should not be here.

     * It is needed because the mapping from 8P line
     * to the device is not done properly.
     */
    int getValueForAddr(char addr);

protected:

    Peripheral* getPeripheralAt(char addr);
};

#endif /* FINREFLEXDM_H_ */

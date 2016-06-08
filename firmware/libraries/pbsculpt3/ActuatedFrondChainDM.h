/*
 * ActuatedFrondChainDM.h
 *
 *  Created on: Jun 7, 2016
 *      Author: dkadish
 */

#ifndef ACTUATEDFRONDCHAINDM_H_
#define ACTUATEDFRONDCHAINDM_H_

#define _N_LEDS 4

#include <deviceModule.h>

class ActuatedFrondChainDM: public DeviceModule {

public:

	ActuatedFrondChainDM(char port);
	virtual ~ActuatedFrondChainDM();

	void init();
	void loop();

	bool fade(int address, int target, int duration);
	int  read(int address, int preprocessType);

	//
	int getPeripheralList(uint8_t* peripheralList, int position);

     /*TODO This is a hack and should not be here.
     * It is needed because the mapping from 8P line
     * to the device is not done properly.
     */
    int getValueForAddr(char addr);

protected:

    Peripheral* getPeripheralAt(char addr);
};

#endif /* ACTUATEDFRONDCHAINDM_H_ */

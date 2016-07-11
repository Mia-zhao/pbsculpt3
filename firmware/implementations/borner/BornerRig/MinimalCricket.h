/*
 * MinimalCricket.h
 *
 *  Created on: Jul 8, 2016
 *      Author: dkadish
 */

#ifndef MINIMALCRICKET_H_
#define MINIMALCRICKET_H_

#include <deviceModule.h>

#include <LinkedList.h>

class MinimalCricket: public DeviceModule {
public:
	MinimalCricket(char port);

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

#endif /* MINIMALCRICKET_H_ */

/* Represents a generic submodule for a Teensy Unit in Series 3
 *
 * author: David Kadish (david.kadish@gmail.com)
 * created: Feb 13, 2016
 *
 */

#ifndef __SUBMODULE_H
#define __SUBMODULE_H

#include <LinkedList.h>

#include "peripheral.h"
#include <Arduino.h>

class DeviceModule {
    
    public:
        
        // Constructor
		DeviceModule(char port);
        
        virtual void init() = 0;
        virtual void loop() = 0;
        
        // 
        virtual int getPeripheralList(uint8_t* peripheralList, int position) = 0;
        
        // Corresponding to the defined communcations protocol
        virtual bool fade(int address, int target, int duration);
        virtual int  read(int address, int preprocessType);
        
        /*TODO This is a hack and should not be here.
         * It is needed because the mapping from 8P line
         * to the device is not done properly.
         */
        virtual int getValueForAddr(char addr) = 0;
        
        virtual int peripheralCount();
        
    protected:
        char _port;
        LinkedList<Peripheral*> peripherals;
};

#endif

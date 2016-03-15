/* Represents a proprioceptive LED subnode in Series 3
 *
 * author: David Kadish (david.kadish@gmail.com)
 * created: Feb 13, 2016
 *
 */

#ifndef __PROPRIOCEPTIVELED_H
#define __PROPRIOCEPTIVELED_H

#include "deviceModule.h"

#include <LinkedList.h>

class ProprioceptiveLed: public DeviceModule {
    
    public:
        
        // Constructor
        ProprioceptiveLed(char port);
        
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

#endif

/* Represents an empty (unpopulated) subnode in Series 3
 *
 * author: David Kadish (david.kadish@gmail.com)
 * created: Feb 13, 2016
 *
 */

#ifndef __EMPTYSUBNODE_H
#define __EMPTYSUBNODE_H

#include "deviceModule.h"

class EmptyDeviceModule: public DeviceModule {
    
    public:
        
        // Constructor
		EmptyDeviceModule(char port);
        
        void init();
        void loop();
        
        // 
        int getPeripheralList(uint8_t* peripheralList, int position);
        
        /*TODO This is a hack and should not be here.
         * It is needed because the mapping from 8P line
         * to the device is not done properly.
         */
        int getValueForAddr(char addr);
        
    protected:
        
        
        
};

#endif

/* Represents a proprioceptive LED subnode in Series 3
 *
 * author: David Kadish (david.kadish@gmail.com)
 * created: Feb 13, 2016
 *
 */

#ifndef __PROPRIOCEPTIVELED_H
#define __PROPRIOCEPTIVELED_H

#include "subnode.h"

class ProprioceptiveLed: public SubNode {
    
    public:
        
        // Constructor
        ProprioceptiveLed(char port);
        
        void init();
        void loop();
        
        bool fade(int address, int target, int duration);
        int  read(int address, int preprocessType);
        
        // 
        void getDeviceList(char* devList);
        
        /*TODO This is a hack and should not be here.
         * It is needed because the mapping from 8P line
         * to the device is not done properly.
         */
        int getValueForAddr(char addr);
        
    protected:
    
        Device* getDeviceAt(char addr);
        
        
        
};

#endif

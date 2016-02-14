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
        
        // 
        void getDeviceList(char* devList);
        
    protected:
        
        
        
};

#endif

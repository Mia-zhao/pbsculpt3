/* Represents an empty (unpopulated) subnode in Series 3
 *
 * author: David Kadish (david.kadish@gmail.com)
 * created: Feb 13, 2016
 *
 */

#ifndef __EMPTYSUBNODE_H
#define __EMPTYSUBNODE_H

#include "subnode.h"

class EmptySubNode: public SubNode {
    
    public:
        
        // Constructor
        EmptySubNode(char port);
        
        void init();
        void loop();
        
        // 
        void getDeviceList(char* devList);
        
    protected:
        
        
        
};

#endif

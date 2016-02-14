/* Represents a Teensy Node in Series 3
 *
 * author: David Kadish (david.kadish@gmail.com)
 * created: Feb 13, 2016
 *
 */

#ifndef __NODE_H
#define __NODE_H

#define N_SUBNODES 6

#include "subnode.h"

class Node {
    
    public:
        
        // Constructor
        Node(int serialNumber);
        
        void init();
        void loop();
        
        SubNode *subnodes[N_SUBNODES];
        
    protected:
        int _serialNumber;
};

#endif

/* Represents a Teensy Node in Series 3
 *
 * author: David Kadish (david.kadish@gmail.com)
 * created: Feb 13, 2016
 *
 */

#ifndef __NODE_H
#define __NODE_H

#define N_DEVICES 6

#include "deviceModule.h"

#include <pins_arduino.h>
#include <Arduino.h>

class Node {
    
    public:
        
        // Constructor
        Node(int serialNumber);
        
        void init();
        void loop();
        
        int deviceCount();
        
        DeviceModule *devices[N_DEVICES];
        
        int _serialNumber;                
        
    protected:
        void switchPort(char port);
};

#endif

/** Represents a Teensy Node in Series 3
 *
 * @author David Kadish (david.kadish@gmail.com)
 * @date Feb 13, 2016
 *
 *
 */

#ifndef __NODE_H
#define __NODE_H

#define N_DEVICES 6

#include "deviceModule.h"

#include <pins_arduino.h>
#include <Arduino.h>

/** Represents a Teensy Node in Series 3
 *
 * Node represents a main node within the system, consisting of a Teensy
 * microcontroller and communication equipment on a circuit board stack.
 * These are often referred to as "Control Nodes" in conversation.
 *
 * @code
 *	#include "node.h"
 *
 *	Node node(111111); // 111111 is the serial number
 *
 *	void setup(){
 *		node.devices[0] = new DeviceModule(1); // The parameter is the 1-indexed
 *		node.devices[1] = new DeviceModule(2); // port number.
 *		node.devices[2] = new DeviceModule(3);
 *		node.devices[3] = new DeviceModule(4); // Note, you could not actually use
 *		node.devices[4] = new DeviceModule(5); // DeviceModule rather, you would
 *		node.devices[5] = new DeviceModule(6); // have to call a subclass
 *	}
 * @endcode
 */
class Node {
    
    public:
        
        /** A main node in a sculpture
         *
         * @param serialNumber The serial number of the Teensy that the
         * firmware is going on. In practice, this can be left as 111111,
         * as the Teensy reads its own serial number on boot.
         *
         * @todo Remove serialNumber as a required parameter
         */
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

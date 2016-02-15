/* Represents a node-level communications device
 *
 * author: David Kadish (david.kadish@gmail.com)
 * created: Feb 14, 2016
 *
 */
 
 #ifndef __NODECOMMUNICATOR_H
 #define __NODECOMMUNICATOR_H
 
 #include <i2c_t3.h>
 
 #include "portCommunicator.h"
 
 class NodeCommunicator {
    public:
        NodeCommunicator();
        
        PortCommunicator getPort(char port_number);
        //Wire getI2C();
    
    protected:
        
        static const PortCommunicator ports[];
        //Wire i2c;
 };
 
 extern NodeCommunicator Comm;
 
 #endif

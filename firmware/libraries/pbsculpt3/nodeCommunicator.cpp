/*
 *
 *
 */

#include "nodeCommunicator.h"

#include <Arduino.h>
 
const PortCommunicator NodeCommunicator::ports[] = {
            PortCommunicator(1),
            PortCommunicator(2),
            PortCommunicator(3),
            PortCommunicator(4),
            PortCommunicator(5),
            PortCommunicator(6),
        };

NodeCommunicator::NodeCommunicator()//:
{
    Wire.begin(I2C_MASTER, 0x00, I2C_PINS_18_19, I2C_PULLUP_EXT, I2C_RATE_400);
}
        
PortCommunicator NodeCommunicator::getPort(char port_number){
    return ports[port_number+1];
}

NodeCommunicator Comm = NodeCommunicator();

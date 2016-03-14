/*
 *
 *
 */

#include "emptySubNode.h"

#include <Arduino.h>

EmptySubNode::EmptySubNode(char port):
    SubNode(port)
{
    
}

void EmptySubNode::init(){}

void EmptySubNode::loop(){
    //Serial.println("EmptySubNode Loop");
}

int EmptySubNode::getDeviceList(uint8_t* devList, int position){
    return position;
}


        
/*TODO This is a hack and should not be here.
 * It is needed because the mapping from 8P line
 * to the device is not done properly.
 */
int EmptySubNode::getValueForAddr(char addr){ return 0; }

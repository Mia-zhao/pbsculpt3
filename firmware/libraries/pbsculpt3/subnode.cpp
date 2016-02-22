/*
 *
 *
 */

#include "subnode.h"
#include "device.h"

#include <Arduino.h>

SubNode::SubNode(char port):
    _port(port)
{
    devices = LinkedList<Device*>();
}

/*void SubNode::loop(){

    /*std::map<int,Device>::iterator iter;
    
    for (iter = devices.begin(); iter != devices.end(); iter++) {
        //Switch to device port
        iter->second.loop();
    }
    
    int n = devices.size();
    for(int i=0; i<n; i++){
        devices[i].loop();
    }
}*/

bool SubNode::fade(int address, int target, int duration){
    return false;
}

int  SubNode::read(int address, int preprocessType){
    return 0;
}

int SubNode::deviceCount(){
    //Serial.printf("Port %i: %i devices...\n", _port, devices.size());
    return devices.size();
}

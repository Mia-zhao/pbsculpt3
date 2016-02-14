/*
 *
 *
 */

#include "subnode.h"
#include "device.h"

SubNode::SubNode(char port):
    _port(port)
{
    
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

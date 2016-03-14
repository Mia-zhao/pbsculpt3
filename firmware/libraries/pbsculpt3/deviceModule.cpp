/*
 *
 *
 */

#include "deviceModule.h"
#include "peripheral.h"

#include <Arduino.h>

DeviceModule::DeviceModule(char port):
    _port(port)
{
    peripherals = LinkedList<Peripheral*>();
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

bool DeviceModule::fade(int address, int target, int duration){
    return false;
}

int  DeviceModule::read(int address, int preprocessType){
    return 0;
}

int DeviceModule::peripheralCount(){
    //Serial.printf("Port %i: %i devices...\n", _port, devices.size());
    return peripherals.size();
}

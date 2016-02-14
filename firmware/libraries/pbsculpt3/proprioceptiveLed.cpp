/*
 *
 *
 */

#include "proprioceptiveLed.h"
#include "highPowerLed.h"

#include <stdio.h>

#define LED_ADDRESS 1
#define PHOTO_ADDRESS 5

ProprioceptiveLed::ProprioceptiveLed(char port):
    SubNode(port)
{
    
}

void ProprioceptiveLed::init(){
    HighPowerLED led(LED_ADDRESS);
    devices.insert(std::pair<int,Device*>(LED_ADDRESS, &led));
}

void ProprioceptiveLed::loop(){
    int n = devices.size();
    for(int i=0; i<n; i++){
        devices[i]->loop();
    }
    
/*    std::map<int,Device*>::iterator iter;
    
    for (iter = devices.begin(); iter != devices.end(); iter++) {
        //Switch to device port
        iter->second->loop();
    }*/
}

void ProprioceptiveLed::getDeviceList(char* devList){
    sprintf(devList, "%i.3.%i %i.6.%i", _port, LED_ADDRESS, _port, PHOTO_ADDRESS);
}

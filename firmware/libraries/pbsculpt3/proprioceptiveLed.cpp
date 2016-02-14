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
    HighPowerLED* led = new HighPowerLED(LED_ADDRESS);
    devices.insert(std::pair<int,Device*>(LED_ADDRESS, led));
}

void ProprioceptiveLed::loop(){
    /*int n = devices.size();
    Serial.printf("Looping propLed for %i devices...\n", n);
    for(int i=0; i<n; i++){
        Serial.printf("Looping propLed: device %i...\n", i);
        devices[i]->loop();
        Serial.printf("Done looping propLed: device %i...\n", i);
    }*/
    Serial.printf("Looping propLed for %i devices...\n", devices.size());
    
    std::map<int,Device*>::iterator iter;
    int i = 0;
    for (iter = devices.begin(); iter != devices.end(); iter++) {
        Serial.printf("Looping propLed: device %i...\n", i);
        i++;
        //Serial.printf("propLed properties: %i, %i, %i, %i\n", iter->second->_value, iter->second->_fadeDuration, iter->second->_fadeTarget, iter->second->_fadeTime);
        //Serial.println(iter->second->address());
        iter->second->loop();
    }
    
    Serial.printf("Done looping propLed...\n");
}

void ProprioceptiveLed::getDeviceList(char* devList){
    sprintf(devList, "%i.3.%i %i.6.%i", _port, LED_ADDRESS, _port, PHOTO_ADDRESS);
}

int ProprioceptiveLed::getValueForAddr(char addr){
    std::map<int,Device*>::iterator iter;
    
    for (iter = devices.begin(); iter != devices.end(); iter++) {
        if( iter->second->address() == addr ){
            return iter->second->read(0);
        }
    }
    
    return 0;
}

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
    devices.add(led);
}

void ProprioceptiveLed::loop(){
    /*int n = devices.size();
    Serial.printf("Looping propLed for %i devices...\n", n);
    for(int i=0; i<n; i++){
        Serial.printf("Looping propLed: device %i...\n", i);
        devices[i]->loop();
        Serial.printf("Done looping propLed: device %i...\n", i);
    }*/
    //Serial.printf("Looping propLed for %i devices...\n", devices.size());
    
    for (int i=0; i<devices.size(); i++) {
        //Serial.printf("Looping propLed: device %i...\n", i);
        //Serial.printf("propLed properties: %i, %i, %i, %i\n", iter->second->_value, iter->second->_fadeDuration, iter->second->_fadeTarget, iter->second->_fadeTime);
        //Serial.println(iter->second->address());
        devices.get(i)->loop();
    }
    
    //Serial.printf("Done looping propLed...\n");
}

bool ProprioceptiveLed::fade(int address, int target, int duration){
    Device* d = getDeviceAt(address);
    if( d != NULL ){
        d->fade(target, duration);
        return true;
    }
    return false;
}

int  ProprioceptiveLed::read(int address, int preprocessType){
    Device* d = getDeviceAt(address);
    if( d != NULL ){
        return d->read(preprocessType);
    }
    return 0;
}


/* Updates the devList array and returns the new position marker
 */
int ProprioceptiveLed::getDeviceList(uint8_t* devList, int position){
   devList[position] = _port;
   devList[position+1] = 3;
   devList[position+2] = LED_ADDRESS;
   devList[position+3] = _port;
   devList[position+4] = 6;
   devList[position+5] = PHOTO_ADDRESS;
   position += 6;
   
   return position;
}

int ProprioceptiveLed::getValueForAddr(char addr){
    Device* d = getDeviceAt(addr);
    if( d != NULL ){
        return d->read(0);
    }
    return 0;
}

Device* ProprioceptiveLed::getDeviceAt(char addr){
    for( int i=0; i<devices.size(); i++ ){
       if( devices.get(i)->address() == addr ){
        return devices.get(i);
       }
    }
    return NULL;
}

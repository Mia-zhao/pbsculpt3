#include "portCommunicator.h"

#define S_PWM_FREQ 1000

PortCommunicator::PortCommunicator(char port_number): _port_number(port_number){
    // Initialize the Teensy ports the way they need to be initialized
    
    for( int addr=0; addr<N_ADDR; addr++ ){
        if(getPortType(addr) == F_PWM){
            pinMode(getTeensyPin(addr), OUTPUT);
        } else if (getPortType(addr) == ANALOG){
            pinMode(getTeensyPin(addr), INPUT);
        }
    }
    
	s_pwm.begin();
	s_pwm.setPWMFreq(S_PWM_FREQ);  // This is the maximum PWM frequency
}

void PortCommunicator::write(char addr, int value){
    switch( getPortType(addr) ){
        case F_PWM:
            analogWrite(getTeensyPin(addr), value);
            break;
        case S_PWM:
            s_pwm.setPWMFast(getTeensyPin(addr), 16*value);
            break;
        case ANALOG:
            // Cannot write anything to an analogRead port
            break;
    }
}

int PortCommunicator::read(char addr){
    switch( getPortType(addr) ){
        case F_PWM:
            return -1;
            break;
        case S_PWM:
            return -1;
            break;
        case ANALOG:
            return analogRead(getTeensyPin(addr));
            break;
    }
}

char PortCommunicator::getTeensyPin(char addr){
    return _port_id_map[_port_number-1][addr];
}

PortType PortCommunicator::getPortType(char addr){
    return _port_type_map[_port_number-1][addr];
}

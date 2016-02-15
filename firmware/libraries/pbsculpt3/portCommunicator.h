/* Represents a port-level communications device
 *
 * author: David Kadish (david.kadish@gmail.com)
 * created: Feb 14, 2016
 *
 */
 
#ifndef __PORTCOMMUNICATOR_H
#define __PORTCOMMUNICATOR_H

#import <i2c_t3.h>
#include "PWMDriver.h"

#define N_ADDR 6
#define N_PORT 6

enum PortType {
    //I2C,
    F_PWM, // Teensy (Fast) PWM
    S_PWM,  // Chip (Slow) PWM
    ANALOG
};

class PortCommunicator {
    public:
        // Initialize the class as a port number, initalize the Teensy ports.
        PortCommunicator(char port_number);
        
        void write(char addr, int value);
        int read(char addr);
        
        char getTeensyPin(char addr);
        PortType getPortType(char addr);
        
    protected:
        
        // 1-indexed port number
        int _port_number;
        
	    PWMDriver s_pwm;
        
    private:
        const PortType _port_type_map[6][6] = {
            {F_PWM, F_PWM, S_PWM, S_PWM, ANALOG, ANALOG},
            {F_PWM, F_PWM, S_PWM, S_PWM, ANALOG, ANALOG},
            {S_PWM, S_PWM, S_PWM, S_PWM, ANALOG, ANALOG},
            {F_PWM, F_PWM, S_PWM, S_PWM, ANALOG, ANALOG},
            {F_PWM, F_PWM, S_PWM, S_PWM, ANALOG, ANALOG},
            {S_PWM, S_PWM, S_PWM, S_PWM, ANALOG, ANALOG}
        };
        
        const char _port_id_map[6][6] = {
            {3, 4, 0, 1, A11, A13},
            {6, 5, 2, 3, A12, A15},
            {4, 5, 6, 7, A16, A17},
            {20, 21, 14, 15, A8, A9},
            {25, 32, 12, 13, A2, A3},
            {8, 9, 10, 11, A0, A1}
        };
    
    
};

#endif

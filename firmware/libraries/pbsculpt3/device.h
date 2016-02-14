/* Represents a generic device for a submodule in Series 3
 *
 * author: David Kadish (david.kadish@gmail.com)
 * created: Feb 13, 2016
 *
 */

#ifndef __DEVICE_H
#define __DEVICE_H

class Device {
    
    public:
        
        // Constructor
        Device(int address, int type);
        
        virtual void init() = 0;
        virtual void loop();
                
        // Corresponding to the defined communcations protocol
        virtual int fade(int target, int duration) = 0;
        virtual int read(int preprocessType) = 0;
        
        int address();
        
    protected:
        int _address;
        int _type;
};

#endif

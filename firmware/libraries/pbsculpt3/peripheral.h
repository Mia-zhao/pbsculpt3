/* Represents a generic device for a submodule in Series 3
 *
 * author: David Kadish (david.kadish@gmail.com)
 * created: Feb 13, 2016
 *
 */

#ifndef __DEVICE_H
#define __DEVICE_H

#define DEVICE_TYPE_HIGH_POWER_LED 5
#define DEVICE_TYPE_PHOTO_SENSOR 6

class Peripheral {
    
    public:
        
        /**Represents a low-level peripheral in a pbsculpt system
         *
         * @brief For example, could be an LED, a proximity sensor, etc.
         *
         * @param address The pin address of the peripheral
         * @param type The (integer) type, defined in the communication spec
         */
		Peripheral(int address, int type);
        
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

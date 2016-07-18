/* Represents a generic submodule for a Teensy Unit in Series 3
 *
 * author: David Kadish (david.kadish@gmail.com)
 * created: Feb 13, 2016
 *
 */

#ifndef __SUBMODULE_H
#define __SUBMODULE_H

#include "LimitedQueue.h"
#include "peripheral.h"
#include <Arduino.h>

class DeviceModule {
    
    public:
        
        // Constructor
		DeviceModule(char port);
        
		/**
		 *
		 *	This function is called by the node when the DeviceModule is first created.
		 *	It is responsible for any initialization that must come after the
		 *	start of the Arduino init() function is called (opening ports,
		 *	an initial serial message, etc.)
		 *
		 *	The implementation of init() in the DeviceModule class calls the
		 *	init() function for every peripheral in the peripherals list and
		 *	should therefore be called at the *end* of the subclassed init()
		 *	function as DeviceModule::init()
		 *
		 */
        virtual void init();

        /**
         *	Called by the Node (or directly from the Arduino loop if a Node
         *	object is not used). DeviceModule::loop() should be called at the
         *	beginning of a subclass's loop function.
         *
         *	The implementation in DeviceModule first calls loop on all of the
         *	Peripherals attached to the DeviceModule. It then handles events
         *	arising from the Peripherals by adding them to its own event queue
         *	make them available to Nodes (or the top level Arduino .ino file).
         *	For each event, it also calls handleDeviceNeighbourEvent() so that
         *  other peripherals on the DM can react to the event.
         */
        virtual void loop();
        
        // 
        virtual int getPeripheralList(uint8_t* peripheralList, int position) = 0;
        
        // Corresponding to the defined communcations protocol
        virtual bool fade(int address, int target, int duration);
        virtual int  read(int address, int preprocessType);
        
        /*TODO This is a hack and should not be here.
         * It is needed because the mapping from 8P line
         * to the device is not done properly.
         */
        virtual int getValueForAddr(char addr) = 0;
        
        virtual int peripheralCount();
        
        bool hasFastPWM();

        bool hasFastPWMForAddress(int address);
        int getPinForAddress(int address);

        LimitedQueue<PeripheralEvent> events;
        elapsedMillis time;

        /** Handle an event originating on this device module
         *
         * When a peripheral that resides on this device module produces an
         * event, this function is used to react to it. For example, if an
         * actuator is reflexive (i.e. responsive to a sensor that is on the
         * same DM, this function can be used to trigger the reflex response.
         *
         * @param e The event object that originated on this DM
         */
        virtual void handleDeviceNeighbourEvent(PeripheralEvent e);

        /** Handle an event originating on this node (but not this DM)
         *
         * This function is called when the DM receives an event originating
         * from this node, but from another device module. This could be used,
         * for example, to trigger a chain reaction of events throughout a node.
         *
         * @param e The event object that originated on this node
         */
        virtual void handleNodeNeighbourEvent(PeripheralEvent e);

        /** Handle an event originating on another node
         *
         * This function is called when the DM receives an event originating
         * on another node. This can be used to trigger global chain reactions
         * or inter-nodal activity.
         *
         * @param e The event object that originated on another node.
         */
        virtual void handleNeighbourEvent(PeripheralEvent e);

    protected:
        char _port;
        LinkedList<Peripheral*> peripherals;
        const uint8_t * _fpwm, * _spwm, * _analog;

        virtual void handleDeviceNeighbourBackgroundActivation(PeripheralEvent e);
        virtual void handleNodeNeighbourBackgroundActivation(PeripheralEvent e);
        virtual void handleNeighbourBackgroundActivation(PeripheralEvent e);

        virtual void handleDeviceNeighbourActivation(PeripheralEvent e);
        virtual void handleNodeNeighbourActivation(PeripheralEvent e);
        virtual void handleNeighbourActivation(PeripheralEvent e);

};

#endif

Attached is the Teensy code for a simulated sculpture.  It can be used as follows:
1. Connected to a PC with the Python CBLA code installed.  The CBLA code will instantiate a single CBLA Node and begin learning the model of the virtual sculpture.  It will also echo any information it gets from the sculpture to a UDP receiver.  It will also send the learning parameters numberExperts and maxActionValue out over UDP at each time step.
2. Connected to a PC and controlled via Grasshopper sending Fade and Read commands.

*For details of the command structure, see the Communication Specification.*

# Implementation details
The sculpture has two LEDs and associated photosensors, and one IR sensor.  The deviceIDs for those are

LED 1: 1 5 3
LED 2: 1 5 4
photosensor 1 (associated with LED1): 1 6 5
photosensor 2 (associated with LED2): 1 6 6
IR sensor: 2 2 6

A fade command to either LED sets the output of that LED immediately to the targetValue; the duration of the fade doesn't do anything.

A read command from either of the photosensors returns the simulated photosensor reading as a 10-bit value (i.e., from 0-1023, calculated as LEDvalue/100*1023)

A read command from the IR sensor returns a random value between 0 and 1023.

The Teensy will ignore commands that are not directed at it, so the software communicating with it will need to know its teensyID.  See the Communication Specification for more details.

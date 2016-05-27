pbsculpt3                         {#mainpage}
============

pbsculpt3 is the name for Version 3.1 and up of the firmware, software, and hardware for the PBAI/LASG sculpture series.
This document focuses on the use of the firmware for that series.

There are three main classes for pbsculpt3: Node, DeviceModule, and Peripheral.
Node represents a main node within the system, consisting of a Teensy microcontroller and communication equipment on a circuit board stack. These are often referred to as "Control Nodes" in conversation.
DeviceModule represents an intermediate circuit board in the system. These boards connect to a parent Node via an 8P modular connection and then break out functionality to Peripheral elements.
Peripheral represents a sensor or actuator within the system. They are wired to a specially designed Device Module and then report back readings and/or perform actuation of an element.

Node, DeviceModule, and Peripheral each have two functions - init() and loop() - and calls to those functions are passed down the chain. Node is responsible for calling init() and loop() for each DeviceModule that it has and likewise, Node is responsible for calling those functions for each Peripheral attached to it.

Documentation on how to use each class is provided in the documentation page for that class.
The <a href="hierarchy.html">Hierarchy</a> page can also be helpful for understanding the structure of the firmware.
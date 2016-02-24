The attached Python code implements a communication protocol for setting and reading from devices attached to a Teensy, based on the Communication Specification.  It echos anything it receives from the sculpture out to a UDP listener.  Currently, the code assumes you are running in Windows.

For it to work, it needs to have three parameters set in simpleTeensyComs.py:
* the teensyID of the Teensy you're communicating with (this is called teensy_sernum in the python code)
* the COM port the Teensy is on (the code currently assumes it is running on a Windows machine; this is called teensy_comport in the code)
* the IP address of the UDP listener (called UDP_IP in the code)

The first two can be determined once the Teensy is plugged into the computer's USB port, by running TyQt as follows
* download and install TyQt from https://github.com/Koromix/ty/releases/
* connect your Teensy to the USB port on your computer (NOTE: Teensy will not work on a USB3.0 port)
* run TyQt
* your Teensy should appear in the panel on the left; select it
* find the Serial Number in the pane on the right; it will always end in 0
* the teensyID is the Serial Number divided by 10 to remove that trailing 0
* find the COM port in the pane on the right under Path

The code also implements a CBLA engine which can be used as an example to learn the virtual test sculpture described elsewhere on Basecamp.
**IT IS NOT FULLY TESTED AND CANNOT BE USED ON ANY SYSTEMS OTHER THAN THE VIRTUAL TEST SYSTEM.**

# Install Instructions
The instructions for installing the Python distribution and tools that this code has been tested with are:
* install the Anaconda3 distribution of Python.  Download Python 3.x; the code won't work with Python 2.x. https://www.continuum.io/downloads
* the IDE we use for Python is PyCharm (Community version), available from https://www.jetbrains.com/pycharm/download/
* once Python is installed, install the serial libraries by typing "pip install serial" at a Windows command prompt (not a Python command prompt)

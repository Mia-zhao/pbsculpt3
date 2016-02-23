# pbsculpt3
Software for PBAI Series 3 Sculptures

# Installation and Setup of EclipseArduino

## Linux (Fedora 23)

### Install Git
`dnf install git`

### Install Eclipse CDT (C Development Tooling)
`dnf install eclipse-cdt`

### Install Arduino 1.6.7
There is an arduino package in the repository, but it is 1.6.4 and thus will not work (see teensyduino notes). Instead, do the following:
1. Download Arduino 1.6.7 (https://downloads.arduino.cc/arduino-1.6.7-linux64.tar.xz)
2. Move the download to the `/usr/local/src/` folder `mv arduino-1.6.7-linux64.tar.xz /usr/local/src`
3. Move to the folder `cd /usr/local/src/`
4. Extract the file `tar -xvf arduino-1.6.7-linux64.tar.xz`
5. Enter the directory `cd arduino-1.6.7`
6. Install `./install.sh`

### Install EclipseArduino
1. Open Eclipse
2. Help > Install New Software
3. Work with: http://eclipse.baeyens.it/nightly > Add
4. Name: EclipseArduino
5. Select Uncategorized > Arduino Eclipse Extensions
6. Next (continue until you can press finish/install)

### Give EclipseArduino write access to /usr/lib64/eclipse/arduinoPlugin
1. `cd /usr/lib64/eclipse/`
2. `sudo mkdir arduinoPlugin`
3. `sudo chown -R $USER:$USER arduinoPlugin/`
4. Restart Eclipse
5. Window > Open Perspective > Arduino, to get into Arduino development mode

### Install Teensyduino
1. Download the installer (https://www.pjrc.com/teensy/td_127/teensyduino.64bit)
2. `chmod u+x teensyduino.64bit`
3. `./teensyduino.64bit` and follow the installer
4. Select the correct installation path (`/usr/local/src/arduino-1.6.7/`, if you followed the arduino install instructions)
5. Select all libraries for installation and install

### Set up Teensy in Eclipse
1. Open the Arduino preferences, Window > Preferences > Arduino
2. Add `/usr/local/src/arduino-1.6.7/libraries` and `/usr/local/src/arduino-1.6.7/hardware/teensy/avr/libraries` to the Private Library path
3. Add `/usr/local/src/arduino-1.6.7/hardware` to the Private hardware path
4. Edit the teensyduino platform.txt file (`/usr/local/src/arduino-1.6.7/hardware/teensy/avr/platform.txt`). Add the following between the 4th and 6th lines: `runtime.tools.avr-gcc.path=/usr/local/src/arduino-1.6.7/hardware/tools/avr`.

Now, you are ready to use EclipseArduino

/** LightActuation.ino
 *
 *	Creates
 *
 */

//136334
/*To Fix

 1. getNumDevices hangs the system
 2. IDs has a zero message size

 */

#define __USE_XBEE__ 1

#include "Arduino.h"
#include "tools.h"
#include "device_types.h"

#include "node.h"
#include "emptyDeviceModule.h"
#include "proprioceptiveLed.h"

#if DEBUG
#include <SerialCommand.h>
SerialCommand sCmd(&Serial);     // The demo SerialCommand object

void cmdQueryNumDevices();
#endif

Node node(111111);

/*void cmdFade();
 void cmdVersion();*/

bool heartbeatOn = false;
elapsedMillis heartbeatTimer;

#if __USE_XBEE__
#include <XBee.h>
XBee xbee = XBee();
XBeeResponse response = XBeeResponse();
// create reusable response objects for responses we expect to handle
ZBRxResponse rx = ZBRxResponse();

uint8_t xbeeSerialNumber[8] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

uint8_t shCmd[] = {'S','H'};// serial low
AtCommandRequest atRequest = AtCommandRequest(shCmd);
AtCommandResponse atResponse = AtCommandResponse();
#endif

void setup() {

	pinMode(LED_BUILTIN, OUTPUT);
	digitalWrite(LED_BUILTIN, HIGH);

	delay(1000);

	for (int i = 0; i < 10; i++) {
		digitalWrite(LED_BUILTIN, LOW);
		delay(50);
		digitalWrite(LED_BUILTIN, HIGH);
		delay(50);
	}

	Serial.begin(9600);
	delay(1000);

	digitalWrite(LED_BUILTIN, LOW);

	DBG("Booting");

	// put your setup code here, to run once:
	node.devices[0] = new ProprioceptiveLed(1);
	DBG(".");
	node.devices[1] = new ProprioceptiveLed(2);
	DBG(".");
	node.devices[2] = new ProprioceptiveLed(3);
	DBGLN(".");
	node.devices[3] = new EmptyDeviceModule(4);
	DBG(".");
	node.devices[4] = new EmptyDeviceModule(5);
	DBG(".");
	node.devices[5] = new EmptyDeviceModule(6);
	DBG(".");

	node.init();
	DBG("Getting Serial Number");
	node._serialNumber = serialCommSetup();
	DBGLN("Finished Booting.");

#if DEBUG
	sCmd.addCommand("NDEV", cmdQueryNumDevices); // Finds the number of devices present.
#endif

#if __USE_XBEE__
	xbee_setup();
#endif

	//Serial.println("Done booting. Awaiting commands, Master...");
}

void loop() {
	// put your main code here, to run repeatedly:
	//Serial.println("Looping...");

	getNumDevices();

	node.loop();
#if DEBUG
	sCmd.readSerial();     // We don't do much, just process serial commands
#else
	serialCommLoop();
#endif

	doHeartbeat();

#if __USE_XBEE__
	xbee_loop();
#endif

	delay(1);
}

#if __USE_XBEE__
void xbee_setup(){
	Serial1.begin(9600);
	delay(1000);
	xbee.begin(Serial1);
	delay(1000);

	xbee_getSerial();
}

void xbee_getSerial(){
	uint8_t slCmd[] = {'S','L'};// serial high

	// Get High Numbers
	sendAtCommand();

	if (atResponse.getValueLength() == 4) {
	  for (int i = 0; i < atResponse.getValueLength(); i++) {
		  xbeeSerialNumber[i] = atResponse.getValue()[i];
	  }
	}

	// Get Low Numbers
	atRequest.setCommand(slCmd);
	sendAtCommand();

	if (atResponse.getValueLength() == 4) {
	  for (int i = 0; i < atResponse.getValueLength(); i++) {
		  xbeeSerialNumber[i+4] = atResponse.getValue()[i];
	  }
	}

	printXBeeSerial();
}

void printXBeeSerial(){
	Serial.print("Serial Number: ");
	for(int i=0; i<8; i++){
		Serial.print(xbeeSerialNumber[i], HEX);
	}
	Serial.println();
}

void sendAtCommand() {
  Serial.println("Sending command to the XBee");

  // send the command
  xbee.send(atRequest);

  // wait up to 5 seconds for the status response
  if (xbee.readPacket(5000)) {
    // got a response!

    // should be an AT command response
    if (xbee.getResponse().getApiId() == AT_COMMAND_RESPONSE) {
      xbee.getResponse().getAtCommandResponse(atResponse);

      if (atResponse.isOk()) {
		  Serial.print("Command [");
		  Serial.print(atResponse.getCommand()[0]);
		  Serial.print(atResponse.getCommand()[1]);
		  Serial.println("] was successful!");

        if (atResponse.getValueLength() > 0) {
        	Serial.print("Command value length is ");
        	Serial.println(atResponse.getValueLength(), DEC);

        	Serial.print("Command value: ");

          for (int i = 0; i < atResponse.getValueLength(); i++) {
        	  Serial.print(atResponse.getValue()[i], HEX);
        	  Serial.print(" ");
          }

          Serial.println("");
        }
      }
      else {
    	  Serial.print("Command return error code: ");
    	  Serial.println(atResponse.getStatus(), HEX);
      }
    } else {
    	Serial.print("Expected AT response but got ");
    	Serial.print(xbee.getResponse().getApiId(), HEX);
    }
  } else {
    // at command failed
    if (xbee.getResponse().isError()) {
    	Serial.print("Error reading packet.  Error code: ");
    	Serial.println(xbee.getResponse().getErrorCode());
    }
    else {
    	Serial.print("No response from radio");
    }
  }
}

void xbee_loop(){
    xbee.readPacket();

    if (xbee.getResponse().isAvailable()) {
        if (xbee.getResponse().getApiId() == ZB_RX_RESPONSE) {
            xbee.getResponse().getZBRxResponse(rx);

            uint8_t * data = rx.getData();

            if(data[0] == 0x00){
            	Serial.println("Ping...");
            } else if (data[0] == 0x01) {
            	Serial.println("Got one...");
            } else if (data[0] == 0x02) {
				printXBeeSerial();
			}
        }
    }
}
#endif


void doHeartbeat() {
	if (heartbeatTimer > 250) {
		heartbeatTimer = 0;

		if (heartbeatOn) {
			digitalWrite(LED_BUILTIN, LOW);
		} else {
			digitalWrite(LED_BUILTIN, HIGH);
		}

		heartbeatOn = !heartbeatOn;
	}
}

/*void cmdFade() {
 //Serial.println("Parsing a fade command...");

 char port = 0, address = 0, target = 0;
 uint16_t duration = 0;
 bool success = false;

 char *arg;

 for (int i = 0; i < 4; i++) {
 arg = sCmd.next();
 if ( arg != NULL ) {
 switch (i) {
 case 0:
 port = atoi(arg);
 break;
 case 1:
 address = atoi(arg);

 serialCommLoop(); break;
 case 2:
 target = atoi(arg);
 break;
 case 3:
 duration = atoi(arg);
 success = true;
 break;
 }
 } else {
 //Serial.printf("Ruh roh. Parameter %i failed", i);
 break;
 //Serial.println("Volume could not be set. Argument was NULL. [" __FILE__ ": " "__LINE__" "]");
 }cmdVersion
 }

 if ( success ) {
 node.subnodes[port - 1]->fade(address, target, duration);
 //Serial.printf( "Done! It worked! FADEd %i to %i for %i millis.\n", address, target, duration);
 }
 }

 void cmdVersion() {
 Serial.println("TEENSY SOFTWARE COMPILED: " __DATE__ " " __TIME__);
 }*/

#define MAX_DEVICES 1000

#define AckRead -4
#define AckFade  -3
#define AckQueryIDs  -2
#define AckQueryNumDevices  -1
#define QueryNumDevices  1
#define QueryIDs  2
#define Fade  3
#define Read  4

static uint8_t teensyID[8];
long int myID;

int portNumber;
int deviceType;
long int destID;
long int origID;
long int messageSize;
char cmdNum;            // uses char type so that it can be signed 7-bit number
uint8_t dataIn[1000];
uint8_t dataOut[1000]; // This is of fixed size at the moment but these should probably be dynamic
int numDevices; // 2 uint8_ts is enough to store the max # of devices = 6x(6+127)=798
unsigned int sensorData; // assumes 2 uint8_ts is big enough to store sensor data
int i;
int deviceValue[2] = { 0, 0 };

void read_teensyID();

long int serialCommSetup() {
	read_teensyID();
	myID = (teensyID[5] << 16) | (teensyID[6] << 8) | (teensyID[7]);
	return myID;
}

//TODO Fix this!
uint8_t getNumDevices() {
	return node.deviceCount();
}

// Load the Device Addresses into an array
// Return the length of the array
//TODO this needs to pull from the device info that we have.
int loadAddresses(uint8_t *addresses) {
	//hacked together. should be replaced.
	// test case:
	for (int i = 0; i < 3; i++) {
		addresses[3 * i] = i + 1 + 3;
		addresses[3 * i + 1] = DEVICE_TYPE_HIGH_POWER_LED;
		addresses[3 * i + 2] = 1;
	}

	for (int i = 0; i < 3; i++) {
		addresses[3*3 + 3 * i] = i + 1 + 3;
		addresses[3*3 + 3 * i + 1] = DEVICE_TYPE_PHOTO_SENSOR;
		addresses[3*3 + 3 * i + 2] = 5;
	}

	return 3 * 3 * 2;
}

//dataIn[0] > port, dataIn[1] > type, dataIn[2] > address, dataIn[3] > value, dataIn[4] > duration
void doFade(uint8_t port, uint8_t type, uint8_t addr, uint8_t value,
		uint8_t duration) {
	node.devices[port - 1]->fade(addr, value, duration);
}

/**
 *
 * @param port The 1-indexed port of the DeviceModule
 * @param type IGNORED for now
 * @param addr The 1-indexed wire address of the Peripheral
 * @param value IGNORED for now
 * @param duration IGNORED for now
 * @return
 */
unsigned int getSensorData(uint8_t port, uint8_t type, uint8_t addr,
		uint8_t value, uint8_t duration) {

	int val = node.devices[port-1]->getValueForAddr(addr);
	return val;
}

// this function assumes that the first sendSize uint8_ts of dataOut have been set already
// it will not send past that, so it doesn't matter if they are valid or not
void sendMessage(long int sendTo, uint8_t sendCmd, long int sendSize) {

	// write the header
	for (i = 2; i >= 0; i--) {
		Serial.write(lowByte(sendTo >> 8 * i));
	}
	for (i = 2; i >= 0; i--) {
		Serial.write(lowByte(myID >> 8 * i));
	}
	Serial.write(sendCmd);
	for (i = 3; i >= 0; i--) {
		Serial.write(lowByte(sendSize >> 8 * i));
	}

	// write the data
	Serial.write(dataOut, sendSize);
	Serial.println();

}

/*void loop() {
 serialCommLoop();
 }*/

void serialCommLoop() {
	if (Serial.available() > 0) {
		DBG("Got a Serial Message...")

		//Serial.println("Serial Available...");

		// get header
		for (i = 2; i >= 0; i--) {
			destID |= (Serial.read() << (8 * i));
		}
		for (i = 2; i >= 0; i--) {
			origID |= (Serial.read() << (8 * i));
		}
		cmdNum = Serial.read();
		for (i = 3; i >= 0; i--) {
			messageSize |= (Serial.read() << (8 * i));
		}

		if (DEBUG)
			Serial.printf("d:%d, o:%d, cmd:%d, size:%d\n", destID, origID,
					cmdNum, messageSize);

		// get the rest of the data
		//    Serial.readuint8_ts(dataIn, messageSize);          // this doesn't work for some reason, though the write in sendMessage works...
		for (i = 0; i < messageSize; i++) {
			dataIn[i] = Serial.read();
		}

		if (myID != destID) {
			// Ignore this message, it isn't for me
			if (DEBUG)
				Serial.printf("Not me!\n");
		} else
		// Figure out what to do

		if (cmdNum == QueryNumDevices) { // return the number of devices on the Teensy Node
			if (DEBUG)
				Serial.printf("Getting NumDevices...\n");
			dataOut[0] = getNumDevices();
			if (DEBUG)
				Serial.printf("Sending num devices %i\n", dataOut[0]);
			sendMessage(origID, -cmdNum, 1);
		} else

		if (cmdNum == QueryIDs) {      // return a structured list of device IDs
			int pos = 0;
			pos = loadAddresses(&dataOut[0]);
			sendMessage(origID, -cmdNum, pos);

		} else

		if (cmdNum == Fade) { // get the extra data and call the appropriate method
			// dataIn[0] > port, dataIn[1] > type, dataIn[2] > address, dataIn[3] > value, dataIn[4] > duration
			if (DEBUG)
				Serial.printf("Fading device %d to %d...\n", dataIn[2],
						dataIn[3]);

			doFade(dataIn[0], dataIn[1], dataIn[2], dataIn[3], dataIn[4]);

			if (DEBUG)
				Serial.printf("New value for LED%d is %d\n", dataIn[2] - 3,
						dataIn[3]);

			// if device is fadeable, set dataOut to be the received Fade data (deviceID,targetvalue,duration)
			dataOut[0] = dataIn[0];
			dataOut[1] = dataIn[1];
			dataOut[2] = dataIn[2];
			dataOut[3] = dataIn[3];
			dataOut[4] = dataIn[4];
			dataOut[5] = dataIn[5];
			// if device is not fadeable, set dataOut[0] to be FF (need to evolve this success flag idea)
			// dataOut[0]=255;
			sendMessage(origID, -cmdNum, 6); // just send an ACK with an empty data field
		} else

		if (cmdNum == Read) {   //get the device number and read/return the data

			sensorData = getSensorData(dataIn[0], dataIn[1], dataIn[2],
					dataIn[3], dataIn[4]);

			dataOut[0] = dataIn[0];
			dataOut[1] = dataIn[1];
			dataOut[2] = dataIn[2];
			dataOut[3] = dataIn[3];
			dataOut[4] = highByte(sensorData);
			dataOut[5] = lowByte(sensorData);
			sendMessage(origID, -cmdNum, 6);
		} else

		if (cmdNum == AckQueryNumDevices) { // device has returned the number of devices it has
			// answer is in dataIn[0]
		} else

		if (cmdNum == AckQueryIDs) {   // device has returned the device ID list
			// answer is in dataIn[0] to dataIn[messageSize-1]
		} else

		if (cmdNum == AckFade) {           // device has returned a success flag
			// success flag is in dataIn[0]
		} else

		if (cmdNum == AckRead) { // device has returned sensor data & success flag
			// success flag is in dagetNumDtaIn[0]
			// data is in dataIn[1] to dataIn[messageSize-1]
		} else {

		}
	}
}

void read_EE(uint8_t word, uint8_t *buf, uint8_t offset) {
	noInterrupts()
	;
	FTFL_FCCOB0 = 0x41;             // Selects the READONCE command
	FTFL_FCCOB1 = word;             // read the given word of read once area

	// launch command and wait until complete
	FTFL_FSTAT = FTFL_FSTAT_CCIF;
	while (!(FTFL_FSTAT & FTFL_FSTAT_CCIF))
		;
	*(buf + offset + 0) = FTFL_FCCOB4;
	*(buf + offset + 1) = FTFL_FCCOB5;
	*(buf + offset + 2) = FTFL_FCCOB6;
	*(buf + offset + 3) = FTFL_FCCOB7;
	interrupts()
	;
}

void read_teensyID() {
	read_EE(0xe, teensyID, 0); // should be 04 E9 E5 xx, this being PJRC's registered OUI
	read_EE(0xf, teensyID, 4); // xx xx xx xx
}

#if DEBUG
// SerialCommand handlers for debugging

void cmdQueryNumDevices() {
	Serial.printf("Finding number of devices.\n");
	delay(100);
	Serial.printf("%d\n", getNumDevices());
}

#endif

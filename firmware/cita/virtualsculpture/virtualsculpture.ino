
/**********************************
This code was written to simulate a virtual sculpture on a Teensy, for debugging and simulation 
using the CBLA python script and the visualization tools.  For help with the communication 
commands see the Communication Specification document.

--

The sculpture has two LEDs and associated photosensors, and one IR sensor.  
The deviceIDs for those are

LED 1: 1 5 3
LED 2: 1 5 4
photosensor 1 (associated with LED1): 1 6 5
photosensor 2 (associated with LED2): 1 6 6
IR sensor: 2 2 6

A fade command to either LED sets the output of that LED immediately to the targetValue; the 
duration of the fade doesn't do anything.

A read command from either of the photosensors returns the simulated photosensor reading as a 
10-bit value (i.e., from 0-1023, calculated as LEDvalue/100*1023)

A read command from the IR sensor returns a random value between 0 and 1023.
**********************************/

//
//

#define AckRead -4
#define AckFade  -3
#define AckQueryIDs  -2
#define AckQueryNumDevices  -1
#define QueryNumDevices  1
#define QueryIDs  2
#define Fade  3
#define Read  4
#define DEBUG 0     // if the teensy is connected to the CBLA or Grasshopper, uncommenting this will mess it up

static uint8_t teensyID[8];
long int myID;



int portNumber;
int deviceType;
long int destID;
long int origID;
long int messageSize;
char cmdNum;            // uses char type so that it can be signed 7-bit number
byte dataIn[1000];
byte dataOut[1000];     // This is of fixed size at the moment but these should probably be dynamic
int numDevices;           // 2 bytes is enough to store the max # of devices = 6x(6+127)=798
unsigned int sensorData;           // assumes 2 bytes is big enough to store sensor data
int i;
int deviceValue[2]={0,0};



void setup() {
  // open serial port
  Serial.begin(9600);
  delay(1000);

  // get the unique Teensy serial number
  read_teensyID();
  myID = (teensyID[5]<<16) | (teensyID[6]<<8) | (teensyID[7]);
  
  if (DEBUG) Serial.printf("Hello from Teensy ser#%d!\n",myID);

  // determine my number of devices
  numDevices = 5;    // 5 devices on the simulated system
}

// this function assumes that the first sendSize bytes of dataOut have been set already
// it will not send past that, so it doesn't matter if they are valid or not
void sendMessage(long int sendTo, byte sendCmd, long int sendSize) {

  // write the header
  for (i=2; i>=0; i--) { Serial.write(lowByte(sendTo>>8*i)); }
  for (i=2; i>=0; i--) { Serial.write(lowByte(myID>>8*i)); }
  Serial.write(sendCmd);
  for (i=3; i>=0; i--) { Serial.write(lowByte(sendSize>>8*i)); }

  // write the data
  Serial.write(dataOut,sendSize);
  Serial.println();
  
}

void loop() {

  if (Serial.available()>0) {

    // get header
    for (i=2;i>=0;i--) {      destID|=(Serial.read()<<(8*i)); }
    for (i=2;i>=0;i--) {      origID|=(Serial.read()<<(8*i)); }
    cmdNum=Serial.read();
    for (i=3;i>=0;i--) { messageSize|=(Serial.read()<<(8*i)); }

    if (DEBUG) Serial.printf("d:%d, o:%d, cmd:%d, size:%d\n",destID,origID,cmdNum,messageSize);
    
    // get the rest of the data
    for (i=0;i<messageSize;i++) { dataIn[i]=Serial.read(); }
    
    if (myID!=destID) {
      // Ignore this message, it isn't for me
      if (DEBUG) Serial.printf("Ignoring message sent to %d.  My teensyID is $d.\n",destID,myID);
    } else
    
      // The command is for me; figure out what to do

      if (cmdNum == QueryNumDevices) {  // return the number of devices on the Teensy Node
          dataOut[0]=numDevices;
          sendMessage(origID,-cmdNum,1);
      } else

      if (cmdNum == QueryIDs) {        // return a structured list of device IDs
        // This is hardcoded for the simulated system
        //  HPLED on port 1, PWM3 
          dataOut[0]=1;
          dataOut[1]=5;
          dataOut[2]=3;
        //  photosensor on port 1, A1
          dataOut[3]=1;
          dataOut[4]=6;
          dataOut[5]=5;
        //  HPLED on port 1, PWM4
          dataOut[6]=1;
          dataOut[7]=5;
          dataOut[8]=4;
        //  photosensor on port 1, A2
          dataOut[9]=1;
          dataOut[10]=6;
          dataOut[11]=6;
        // IR sensor on port 2, A2
          dataOut[12]=2;
          dataOut[13]=2;
          dataOut[14]=6;
          
          sendMessage(origID,-cmdNum,numDevices*3);
          
      } else
      
      if (cmdNum == Fade) {            // call the appropriate Fade method

        // The following lines are to simulate the fading, for this virtual sculpture
        // the appropriate LED is set immediately to the target value
        if (DEBUG) Serial.printf("Fading device %d to %d...\n",dataIn[2],dataIn[3]);
        deviceValue[dataIn[2]-3]=dataIn[3];   // Set the LEDs to the targetvalue for Dana's test
        if (DEBUG) Serial.printf("New value for LED%d is %d\n",dataIn[2]-3,dataIn[3]);
        
        // if device is fadeable, echo back the fade data
        dataOut[0]=dataIn[0];
        dataOut[1]=dataIn[1];
        dataOut[2]=dataIn[2];
        dataOut[3]=dataIn[3];
        dataOut[4]=dataIn[4];
        dataOut[5]=dataIn[5];
        // if device is not fadeable, send back some kind of error message (not implemented)
        // dataOut[0] = // put error message code here
        
        sendMessage(origID,-cmdNum,6); 
      } else

      if (cmdNum == Read) {        // read & return the sensor data

        // this simulates a read from the photosensors in the virtual sculpture
        if (dataIn[1]==6) {
          if (DEBUG) Serial.printf("Reading from LED%d: ",dataIn[2]-3);
          sensorData = int(float(deviceValue[dataIn[2]-5])/100*1023);
          if (DEBUG) Serial.printf("LED setting=%d, sensor reading=%d\n",sensorData);
        }

        // this simulates a read from the IR sensor in the virtual sculpture
        if (dataIn[1]==2) sensorData = random(1023);

        // echo back the deviceID and preprocessing method, and the sensor value
        dataOut[0]=dataIn[0];
        dataOut[1]=dataIn[1];
        dataOut[2]=dataIn[2];
        dataOut[3]=dataIn[3];
        dataOut[4]=highByte(sensorData);
        dataOut[5]=lowByte(sensorData);
        
        sendMessage(origID,-cmdNum,6);
      } else

      // response to Acks are not implemented on the Teensy, since it doesn't currently act as a master
      
      if (cmdNum == AckQueryNumDevices)  {      // device has returned the number of devices it has
        // answer is in dataIn[0]
      } else

      if (cmdNum == AckQueryIDs)  {         // device has returned the device ID list
        // answer is in dataIn[0] to dataIn[messageSize-1]
      } else

      if (cmdNum == AckFade)  {           // device has returned a success flag
        // success flag is in dataIn[0]
      } else

      if (cmdNum == AckRead)  {         // device has returned sensor data & success flag
        // success flag is in dataIn[0]
        // data is in dataIn[1] to dataIn[messageSize-1]
      } else {
  
      }
    }

}

void read_EE(uint8_t word, uint8_t *buf, uint8_t offset)  {
  noInterrupts();
  FTFL_FCCOB0 = 0x41;             // Selects the READONCE command
  FTFL_FCCOB1 = word;             // read the given word of read once area

  // launch command and wait until complete
  FTFL_FSTAT = FTFL_FSTAT_CCIF;
  while(!(FTFL_FSTAT & FTFL_FSTAT_CCIF))
    ;
  *(buf+offset+0) = FTFL_FCCOB4;
  *(buf+offset+1) = FTFL_FCCOB5;       
  *(buf+offset+2) = FTFL_FCCOB6;       
  *(buf+offset+3) = FTFL_FCCOB7;       
  interrupts();
}

void read_teensyID() {
  read_EE(0xe,teensyID,0); // should be 04 E9 E5 xx, this being PJRC's registered OUI
  read_EE(0xf,teensyID,4); // xx xx xx xx
}



